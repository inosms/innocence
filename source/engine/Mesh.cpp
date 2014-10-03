#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Mesh.h"
#include "Math.h"
#include "Error.h"
#include "Shader.h"
#include "Application.h"
#include "VideoSystem_SDL_OpenGL.h"
// http://assimp.sourceforge.net/lib_html/usage.html
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

MeshNodeManager g_meshNodeManager;


glm::mat4 toMat4(aiMatrix4x4 n_mat)
{
	//https://stackoverflow.com/questions/20803441/skeletal-animation-ussing-assimp-and-glsl-bone-uniform-array-size
	return glm::transpose(glm::make_mat4(&n_mat.a1));
}

// creates the MeshNode hierarchie from a aiNode hierarchy
// if n_node is null this returns the root MeshNode for a new hierarchy
// if n_node is a node, then the current n_aiNode contents are added as meshes
// and all the nodes of aiNode are added as children to n_node
MeshNode* BuildNodeHierachy(MeshNode* n_node, aiNode* n_aiNode, std::vector<Mesh*> &n_meshes)
{
	// if no node; create this as root node
	if( !n_node )
		n_node = new MeshNode(toMat4(n_aiNode->mTransformation));

	// set identifier for identification for animation
	n_node->SetIdentifier(std::string(n_aiNode->mName.C_Str()));

	// add all meshes of the aiNode to this node
	for( unsigned int i = 0; i < n_aiNode->mNumMeshes && i < 1; i++ )
	{
		unsigned int tmp_meshIndex = n_aiNode->mMeshes[i];
		Mesh* tmp_meshAtIndex = n_meshes[tmp_meshIndex];
		n_node->AddMesh(tmp_meshAtIndex);
	}
	// for all children
	for( unsigned int i = 0; i < n_aiNode->mNumChildren; i++ )
	{
		aiNode* tmp_ithNode = n_aiNode->mChildren[i];
		if( tmp_ithNode->mNumMeshes == 0 && tmp_ithNode->mNumChildren == 0 ) continue;
		MeshNode* tmp_newChild = new MeshNode(toMat4(tmp_ithNode->mTransformation));
		n_node->AddChild(tmp_newChild);
		BuildNodeHierachy(tmp_newChild,tmp_ithNode,n_meshes);
	}

	return n_node;
}

MeshNode* Mesh::LoadMeshFromFile(std::string n_filename)
{
	Assimp::Importer tmp_importer;
	const aiScene* tmp_scene = tmp_importer.ReadFile(n_filename, aiProcessPreset_TargetRealtime_Quality);
	if( !tmp_scene ) throw Exception(tmp_importer.GetErrorString());

	std::vector<Mesh*> tmp_meshes;
	// iterate over all meshes and create all meshes
	for( unsigned int i = 0; i < tmp_scene->mNumMeshes; i++ )
	{
		const aiMesh* tmp_mesh = tmp_scene->mMeshes[i];

		// create the index array
		std::vector<unsigned int> tmp_indicesArray;
		for( unsigned int j = 0; j < tmp_mesh->mNumFaces; j++ )
		{
			// this also assumes triangles
			tmp_indicesArray.push_back(tmp_mesh->mFaces[j].mIndices[0]);
			tmp_indicesArray.push_back(tmp_mesh->mFaces[j].mIndices[1]);
			tmp_indicesArray.push_back(tmp_mesh->mFaces[j].mIndices[2]);
		}
			/* TODO: the texture coordinates!!
		float* tmp_texture

		// buffer for vertex texture coordinates
		if (mesh->HasTextureCoords(0)) {
			float *texCoords = (float *)malloc(sizeof(float)*2*mesh->mNumVertices);
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

				texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
				texCoords[k*2+1] = mesh->mTextureCoords[0][k].y;

			}
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
			glEnableVertexAttribArray(texCoordLoc);
			glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
		}*/

		Mesh* tmp_newMesh = new Mesh((float*)tmp_mesh->mVertices,nullptr,(float*)tmp_mesh->mNormals,nullptr,tmp_indicesArray,tmp_mesh->mNumVertices*3);
		tmp_meshes.push_back(tmp_newMesh);
	}
	// create the whole hierarchy
	MeshNode* tmp_rootNode = BuildNodeHierachy(nullptr,tmp_scene->mRootNode,tmp_meshes);

	// add all the meshes in order that the root node can free them
	// when the root node is deleted
	tmp_rootNode->SetMeshRootList(tmp_meshes);

	// load the animation
	for( unsigned int i = 0; i < tmp_scene->mNumAnimations; i++ )
	{
		aiAnimation* tmp_animation = tmp_scene->mAnimations[i];
		Animation tmp_newAnimation(std::string(tmp_animation->mName.C_Str()),tmp_animation->mDuration);
		for( unsigned int j = 0; j < tmp_animation->mNumChannels; j++)
		{
			aiNodeAnim* tmp_nodeAnimation = tmp_animation->mChannels[j];
			std::string tmp_identifier = std::string(tmp_nodeAnimation->mNodeName.C_Str());
			Animatable* tmp_animatedThing = tmp_rootNode->VFindAnimatable(tmp_identifier);
			if(!tmp_animatedThing)
			{
				ERROR_MESSAGE("cannot load animation " << tmp_identifier << " as there exists no node for it");
				break;
			}
			Keyframes tmp_newKeyframes;
			for( unsigned int k = 0; k < tmp_nodeAnimation->mNumPositionKeys; k++ )
			{
				aiVectorKey* tmp_key = &tmp_nodeAnimation->mPositionKeys[k];
				glm::vec3 tmp_newPosition(tmp_key->mValue.x,tmp_key->mValue.y,tmp_key->mValue.z);
				double tmp_newTime = tmp_key->mTime;
				tmp_newKeyframes.AddKeyframe(glm::translate(glm::mat4(1.f),tmp_newPosition),tmp_newTime);
			}
			tmp_newAnimation.AddKeyframesForAnimatable(tmp_animatedThing,tmp_newKeyframes);
		}
		tmp_rootNode->GetAnimationManagerReference().AddAnimation(tmp_newAnimation);
	}
	tmp_rootNode->GetAnimationManagerReference().SetActiveAnimation("");
	return tmp_rootNode;
}

Mesh::~Mesh()
{
	if( m_texture ) delete m_texture;
}

Mesh::Mesh( float* n_vertices, float* n_colors, float* n_normals, float* n_textureCoords, std::vector<unsigned int> n_indices, unsigned int n_size )
{
	// this is NOT size/3, as shared vertices are possible!
	// as verticesCount is the number of vertices that are drawn
	// this is exactly the number of elements in the indices array
	m_verticesCount = n_indices.size();
	if( !n_vertices ) throw std::string("no vertices given!");

	// create new Vertex Array Object
	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);

		// in case there was no indices vector given
		// generate one;
		// it assumes that all vertices are given in the order
		// 1 2 3 4,... and size/3 is the number of vertices
		// (no shared vertices)
		std::vector<unsigned int> tmp_indices = n_indices;
		if( tmp_indices.size() == 0 )
		{
			// also set the count to size/3
			// (as no shared vertices are assumed)
			m_verticesCount = n_size/3;
			for( unsigned int i = 0; i < m_verticesCount; i++ )
				tmp_indices.push_back(i);
		}

		// create new element buffer
		unsigned int tmp_elementBuffer;
		glGenBuffers(1,&tmp_elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,tmp_elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * tmp_indices.size(), &tmp_indices[0], GL_STATIC_DRAW);

		// generate vertices buffer
		// there must at least be given vertices
		glGenBuffers(1,&m_vbo[0]);
		glBindBuffer(GL_ARRAY_BUFFER,m_vbo[0]);
		glBufferData(GL_ARRAY_BUFFER,n_size*sizeof(GLfloat),n_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0,3,GL_FLOAT,GL_FALSE,0,0);
		glEnableVertexAttribArray(0);

		// generate color and normals only when given
		if( n_colors )
		{
			glGenBuffers(1,&m_vbo[1]);
			glBindBuffer(GL_ARRAY_BUFFER,m_vbo[1]);
			glBufferData(GL_ARRAY_BUFFER,n_size*sizeof(GLfloat),n_colors,GL_STATIC_DRAW);
			glVertexAttribPointer((GLuint)1,3,GL_FLOAT,GL_FALSE,0,0);
			glEnableVertexAttribArray(1);
		}
		if( n_normals )
		{
			glGenBuffers(1,&m_vbo[2]);
			glBindBuffer(GL_ARRAY_BUFFER,m_vbo[2]);
			glBufferData(GL_ARRAY_BUFFER,n_size*sizeof(GLfloat),n_normals,GL_STATIC_DRAW);
			glVertexAttribPointer((GLuint)2,3,GL_FLOAT,GL_FALSE,0,0);
			glEnableVertexAttribArray(2);
		}
		if( n_textureCoords )
		{
			glGenBuffers(1,&m_vbo[3]);
			glBindBuffer(GL_ARRAY_BUFFER,m_vbo[3]);
			// (n_size/3)*2 because for each vertex there must only be TWO tex coords
			// as n_size/3 is the number of vertices; this results in the correct number
			glBufferData(GL_ARRAY_BUFFER,(n_size/3)*2*sizeof(GLfloat),n_textureCoords,GL_STATIC_DRAW);
			glVertexAttribPointer((GLuint)3,2,GL_FLOAT,GL_FALSE,0,0);
			glEnableVertexAttribArray(3);
		}

	glBindVertexArray(0);
}

void Mesh::Render()
{
	if( m_texture ) m_texture->Bind(0);
	glBindVertexArray(m_vao);
		glDrawElements(GL_TRIANGLES,m_verticesCount*3,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}

void Mesh::SetTexture(Texture* n_texture)
{
	m_texture = n_texture;
}

Texture* Mesh::GetTexture()
{
	return m_texture;
}

Mesh* Mesh::GetRect()
{
	float n_width = 1;
	float n_height = 1;
	float n_centerY = 0;
	float n_centerX = 0;
	float n_vert[] = {-n_width/2+n_centerX,-n_height/2+n_centerY,0.f,
					n_width/2+n_centerX,-n_height/2+n_centerY,0.f,
					n_width/2+n_centerX,n_height/2+n_centerY,0.f,
					n_width/2+n_centerX,n_height/2+n_centerY,0.f,
					-n_width/2+n_centerX,n_height/2+n_centerY,0.f,
					-n_width/2+n_centerX,-n_height/2+n_centerY,0.f};
	float n_color[] = { 1,1,1,
						1,1,1,
						1,1,1,
						1,1,1,
						1,1,1,
						1,1,1};
	Mesh* tmp_new = new Mesh(n_vert,n_color,nullptr,nullptr,std::vector<unsigned int>(),18);
	return tmp_new;
}

void Keyframes::AddKeyframe(glm::mat4 n_keyframeMatrix, double n_time)
{
	Keyframes::Keyframe tmp_newKeyframe;
	tmp_newKeyframe.matrix = n_keyframeMatrix;
	tmp_newKeyframe.time = n_time;
	m_keyframes.push_back(tmp_newKeyframe);
}

// FIXME: if n_time is bigger than biggest keyframe time
// but it seems that assimp shrinks mDuration to the
// maximum of the last keyframe...
glm::mat4 Keyframes::GetAnimationMatrix(double n_time)
{
	unsigned tmp_smallerIndex = m_keyframes.size()-1;
	for( unsigned int i = 0; i < m_keyframes.size(); i++ )
	{
		if( m_keyframes[i].time > n_time ) break;
		else tmp_smallerIndex = i;
	}
	unsigned int tmp_biggerIndex = (tmp_smallerIndex+1)%m_keyframes.size();

	Keyframe tmp_prev = m_keyframes[tmp_smallerIndex];
	Keyframe tmp_next = m_keyframes[tmp_biggerIndex];

	double tmp_timeDiff = fmax(0.f,n_time - tmp_prev.time);
	double tmp_diffToLastStep = tmp_next.time - tmp_prev.time;
	glm::mat4 tmp_diffMatrix = tmp_next.matrix - tmp_prev.matrix;
	double tmp_interpolationValue = tmp_timeDiff/tmp_diffToLastStep;
	return tmp_prev.matrix + float(tmp_interpolationValue)*tmp_diffMatrix;
}

void Animatable::SetIdentifier(std::string n_identifier){m_identifier = n_identifier;}
bool Animatable::IsIdentifier(std::string n_identifier){return m_identifier==n_identifier;}

Animation::Animation(std::string n_name,double n_duration) :
	m_name(n_name),
	m_duration(n_duration)
{
	INFO(m_name);
}

void Animation::Tick(unsigned int n_ticksPerSecond)
{
	double tmp_timePerTick = 1000.0/n_ticksPerSecond;
	m_time += 0.001;//tmp_timePerTick;

	// reset the time after one complete animation
	if( m_time > m_duration )
		m_time -= m_duration;
}

void Animation::Apply(double n_interpolation)
{
	for( auto i : m_keyframes )
	{
		i.first->VSetAnimationMatrix(i.second.GetAnimationMatrix(m_time));
	}
}


void Animation::AddKeyframesForAnimatable(Animatable* n_animatable,Keyframes n_keyframes)
{
	m_keyframes[n_animatable] = n_keyframes;
}


MeshNode::MeshNode(glm::mat4 n_matrix) :
	m_matrix(n_matrix)
{}

MeshNode::~MeshNode()
{
	// delete all children
	for( auto i : m_children )
		delete i;
	// delete all meshes for children
	for( auto i : m_rootMeshList )
		delete i;
}

void MeshNode::Render(Shader* n_shader, glm::mat4 n_relativeMat )
{
	// set correct matrix
	glm::mat4 tmp_thisNewMatrix = n_relativeMat * m_matrix;
	if( n_shader )
	{
		n_shader->SetMat("modelview",tmp_thisNewMatrix);
		n_shader->SetMat("normalMat", tmp_thisNewMatrix);//glm::inverseTranspose(tmp_thisNewMatrix));
	}

	// default render the meshes
	for( auto i : m_meshes )
	 	i->Render();

	// render children
	for( auto i : m_children )
		i->Render(n_shader,tmp_thisNewMatrix);
}

void MeshNode::AddChild(MeshNode* n_child)
{
	m_children.push_back(n_child);
}

void MeshNode::AddMesh(Mesh* n_mesh)
{
	m_meshes.push_back(n_mesh);
}

void MeshNode::SetMeshRootList(std::vector<Mesh*> n_rootMeshList)
{
	m_rootMeshList = n_rootMeshList;
}

void AnimationManager::AddAnimation(Animation n_animation)
{
	m_animations.push_back(n_animation);
}

void AnimationManager::SetActiveAnimation(std::string n_name)
{
	for( int i = 0; i < m_animations.size(); i++ )
	{
		if( m_animations[i].m_name == n_name )
		{
			m_activeAnimation = i;
			return;
		}
	}
	// if animation was not found set it to invalid index
	m_activeAnimation = -1;
	ERROR_MESSAGE("can not set animation " << n_name << " as active because it does not exist");
}

void AnimationManager::Tick()
{
	// if invalid index return
	if( m_activeAnimation == -1 ) return;

	// calculate ticks per second
	unsigned int tmp_ticksPerSecond = 1000/g_application->GetGameLogicTicksDelay();

	m_animations[m_activeAnimation].Tick(tmp_ticksPerSecond);
}
void AnimationManager::Apply(double n_interpolation)
{
	// if invalid index return
	if( m_activeAnimation == -1 ) return;

	m_animations[m_activeAnimation].Apply(n_interpolation);
}

AnimationManager& MeshNode::GetAnimationManagerReference()
{
	return m_animationManager;
}

AnimationManager MeshNode::GetAnimationManagerCopy()
{
	return m_animationManager;
}


void MeshNode::VSetAnimationMatrix(glm::mat4 n_mat)
{
	m_matrix = n_mat;
}

Animatable* MeshNode::VFindAnimatable(std::string n_identifier)
{
	if( IsIdentifier(n_identifier) ) return this;
	for( auto i : m_children )
	{
		Animatable* tmp_find = i->VFindAnimatable(n_identifier);
		if( tmp_find != nullptr) return tmp_find;
	}
	return nullptr;
}

MeshTexture* Mesh::GetTexturedRect(float n_width, float n_height, Texture* n_texture)
{
	return Mesh::GetTexturedRect(n_width,n_height,n_texture,1,1);
}

MeshTexture* Mesh::GetTexturedRect(float n_width, float n_height, float n_centerX, float n_centerY, Texture* n_texture)
{
	return Mesh::GetTexturedRect(n_width,n_height,n_centerX,n_centerY,n_texture,1,1);
}

MeshTexture* Mesh::GetTexturedRect(float n_width, float n_height,Texture* n_texture, float n_textureXScale, float n_textureYScale)
{
	return Mesh::GetTexturedRect(n_width,n_height,0,0,n_texture,n_textureXScale,n_textureYScale);
}

MeshTexture* Mesh::GetTexturedRectProportionalByWidth(float n_width,float n_centerX, float n_centerY, Texture* n_texture, float n_textureXScale, float n_textureYScale)
{
	float tmp_ratio = n_texture->GetHeight()/float(n_texture->GetWidth());
	float tmp_proportionalHeight = tmp_ratio * n_width;
	return Mesh::GetTexturedRect(n_width,tmp_proportionalHeight,n_centerX,n_centerY,n_texture,n_textureXScale,n_textureYScale);
}

MeshTexture* Mesh::GetTexturedRectProportionalByHeight(float n_height,float n_centerX, float n_centerY, Texture* n_texture, float n_textureXScale, float n_textureYScale)
{
	float tmp_ratio = n_texture->GetWidth()/float(n_texture->GetHeight());
	float tmp_proportionalWidth = tmp_ratio * n_height;
	return Mesh::GetTexturedRect(tmp_proportionalWidth,n_height,n_centerX,n_centerY,n_texture,n_textureXScale,n_textureYScale);
}

MeshTexture* Mesh::GetTexturedRect(float n_width, float n_height, float n_centerX, float n_centerY, Texture* n_texture,float n_textureXScale, float n_textureYScale)
{
	float n_vert[] = {-n_width/2+n_centerX,-n_height/2+n_centerY,0.f,
					  n_width/2+n_centerX,-n_height/2+n_centerY,0.f,
					  n_width/2+n_centerX,n_height/2+n_centerY,0.f,
					  n_width/2+n_centerX,n_height/2+n_centerY,0.f,
					  -n_width/2+n_centerX,n_height/2+n_centerY,0.f,
					  -n_width/2+n_centerX,-n_height/2+n_centerY,0.f};
	float n_tex[] = {0.f,n_textureYScale,
					 n_textureXScale,n_textureYScale,
					 n_textureXScale,0,
					 n_textureXScale,0,
					 0.f,0,
					 0.f,n_textureYScale};
	MeshTexture* tmp_new = new MeshTexture(n_width, n_height, n_centerX,n_centerY,n_texture,n_textureXScale,n_textureYScale, n_vert,nullptr,nullptr,n_tex,18);
	tmp_new->SetTexture(n_texture);
	return tmp_new;
}

MeshTexture::MeshTexture(float n_width, float n_height, float n_centerX, float n_centerY, Texture* n_texture,float n_textureXScale, float n_textureYScale,float* n_vertices, float* n_colors, float* n_normals, float* n_textureCoords, unsigned int n_size) :
	Mesh(n_vertices,n_colors,n_normals,n_textureCoords,std::vector<unsigned int>(),n_size),
	m_width(n_width),
	m_height(n_height),
	m_centerX(n_centerX),
	m_centerY(n_centerY),
	m_textureXScale(n_textureXScale),
	m_textureYScale(n_textureYScale)
{

}

void MeshNodeManager::AddMeshNode(std::string n_name, MeshNode* n_mesh)
{
	m_map[n_name] = n_mesh;
}

MeshNode* MeshNodeManager::GetMeshNode(std::string n_name)
{
	if( m_map.find(n_name) == m_map.end() )
		return nullptr;
	else
		return m_map[n_name];
}
