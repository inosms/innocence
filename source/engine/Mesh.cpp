#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Mesh.h"
#include "Math.h"
#include "Error.h"
#include "VideoSystem_SDL_OpenGL.h"
// http://assimp.sourceforge.net/lib_html/usage.html
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

MeshManager g_meshManager;

// https://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
std::vector<std::string> split(std::string n_string, char n_delim)
{
	std::stringstream ss(n_string);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, n_delim)) {
        elems.push_back(item);
    }
    return elems;
}


int toInt(std::string n_string)
{
	std::stringstream s(n_string);
	int tmp_int;
	s >> tmp_int;
	return tmp_int;
}

Mesh::Mesh(std::string n_filename)
{
	Assimp::Importer tmp_importer;
	const aiScene* tmp_scene = tmp_importer.ReadFile(n_filename, aiProcessPreset_TargetRealtime_Quality);
	if( !tmp_scene ) throw Exception(tmp_importer.GetErrorString());
	INFO(n_filename << " has " << tmp_scene->mNumMeshes << " meshes ");

	// so this loop just loops once, but it actually should loop for every mesh
	// and create every mesh; and also store it in a SceneNode Hierarchy, so
	// have fun coding that
	// hue
	// ... that's me... so I've started writing with myself in my code...
	for( unsigned int i = 0; i < tmp_scene->mNumMeshes && i < 1 /* FIXME!! */; i++ )
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

		Init((float*)tmp_mesh->mVertices,nullptr,(float*)tmp_mesh->mNormals,nullptr,tmp_indicesArray,tmp_mesh->mNumVertices*3);
		return; // FIXME: this should definitely not be here...
			/*
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
	}
}

Mesh::~Mesh()
{
	if( m_texture ) delete m_texture;
}

void Mesh::Init(float* n_vertices, float* n_colors, float* n_normals, float* n_textureCoords,std::vector<unsigned int> n_indices, int n_size )
{
	m_verticesCount = n_indices.size();
	if( !n_vertices ) throw std::string("no vertices given!");

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
			glBufferData(GL_ARRAY_BUFFER,(n_size/3)*2*sizeof(GLfloat),n_textureCoords,GL_STATIC_DRAW);
			glVertexAttribPointer((GLuint)3,2,GL_FLOAT,GL_FALSE,0,0);
			glEnableVertexAttribArray(3);
		}

	glBindVertexArray(0);
}

Mesh::Mesh( float* n_vertices, float* n_colors, float* n_normals, float* n_textureCoords, std::vector<unsigned int> n_indices, unsigned int n_size )
{
	Init(n_vertices,n_colors,n_normals,n_textureCoords,n_indices,n_size);
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

void MeshManager::AddMesh(std::string n_name, Mesh* n_mesh)
{
	m_map[n_name] = n_mesh;
}

Mesh* MeshManager::GetMesh(std::string n_name)
{
	if( m_map.find(n_name) == m_map.end() )
		return nullptr;
	else
		return m_map[n_name];
}
