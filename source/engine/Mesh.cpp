#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Mesh.h"
#include "Math.h"
#include "Error.h"
#include "Shader.h"
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
		n_shader->SetMat("normalMat", glm::inverseTranspose(tmp_thisNewMatrix));
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
