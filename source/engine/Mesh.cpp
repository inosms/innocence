#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "Mesh.h"
#include "Math.h"
#include "VideoSystem_SDL_OpenGL.h"

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
	// load obj file and parse
	std::fstream tmp_inputFile(n_filename,std::ios::in);

	std::vector<glm::vec3> tmp_vertices;
	std::vector<glm::vec3> tmp_normals;

	std::vector<glm::vec3> tmp_verticesInCorrectOrder;
	std::vector<glm::vec3> tmp_normalsInCorrectOrder;

	if( tmp_inputFile.is_open() )
	{
		while( !tmp_inputFile.eof() )
		{
			std::string tmp_command;
			tmp_inputFile >> tmp_command;

			// vertex
			if( tmp_command == "v" )
			{
				glm::vec3 tmp_newVert;
				tmp_inputFile >> tmp_newVert.x;
				tmp_inputFile >> tmp_newVert.y;
				tmp_inputFile >> tmp_newVert.z;
				tmp_vertices.push_back(tmp_newVert);
			}
			// normal
			else if( tmp_command == "vn" )
			{
				glm::vec3 tmp_newNormal;
				tmp_inputFile >> tmp_newNormal.x;
				tmp_inputFile >> tmp_newNormal.y;
				tmp_inputFile >> tmp_newNormal.z;
				tmp_normals.push_back(tmp_newNormal);
			}
			else if( tmp_command == "f" )
			{
				for( int i = 0; i < 3; i++ )
				{
					std::string tmp_vert;
					tmp_inputFile >> tmp_vert;
					std::vector<std::string> tmp_indices = split(tmp_vert,'/');
					tmp_verticesInCorrectOrder.push_back(tmp_vertices[toInt(tmp_indices[0])-1]);
					tmp_normalsInCorrectOrder.push_back(tmp_normals[toInt(tmp_indices[2])-1]);
				}				
			}
		}
	}
	else throw std::string("file not found");   

	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao); 

	// generate vertices buffer
	// there must at least be given vertices
	glGenBuffers(1,&m_vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER,tmp_verticesInCorrectOrder.size()*sizeof(glm::vec3),&tmp_verticesInCorrectOrder[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1,&m_vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER,m_vbo[2]);
	glBufferData(GL_ARRAY_BUFFER,tmp_normalsInCorrectOrder.size()*sizeof(glm::vec3),&tmp_normalsInCorrectOrder[0],GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2,3,GL_FLOAT,GL_FALSE,0,0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	m_verticesCount = tmp_verticesInCorrectOrder.size();
}

Mesh::Mesh( float* n_vertices, float* n_colors, float* n_normals, float* n_textureCoords, unsigned int n_size ) :
	m_verticesCount(n_size/3)
{
	if( !n_vertices ) throw std::string("no vertices given!");

	glGenVertexArrays(1,&m_vao);
	glBindVertexArray(m_vao);

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

void Mesh::Render()
{
	if( m_texture ) m_texture->Bind(0);
	glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES,0,m_verticesCount);
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

MeshTexture::MeshTexture(float n_width, float n_height, float n_centerX, float n_centerY, Texture* n_texture,float n_textureXScale, float n_textureYScale,float* n_vertices, float* n_colors, float* n_normals, float* n_textureCoords, unsigned int n_size) : Mesh(n_vertices,n_colors,n_normals,n_textureCoords,n_size),
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