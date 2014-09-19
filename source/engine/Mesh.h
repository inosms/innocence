#ifndef _MESH_H_
#define _MESH_H_

#include <string>
#include <map>
#include "Texture.h"

class MeshTexture;
class Mesh
{
	// 4, because:
	// vertex, color, normal, texture
	unsigned int m_vbo[4];
	unsigned int m_vao;

	unsigned int m_verticesCount;

	Texture* m_texture = nullptr;
public:

	// init via obj file
	Mesh(std::string n_filename);

	// init via arrays
	Mesh( float* n_vertices, float* n_colors, float* n_normals, float* n_textureCoords, unsigned int n_size );

	void Render();

	void SetTexture(Texture* n_texture);
	Texture* GetTexture();

	static MeshTexture* GetTexturedRect(float n_width, float n_height, Texture* n_texture);
	static MeshTexture* GetTexturedRect(float n_width, float n_height, float n_centerX, float n_centerY, Texture* n_texture);
	static MeshTexture* GetTexturedRect(float n_width, float n_height, Texture* n_texture,float n_textureXScale, float n_textureYScale);
	static MeshTexture* GetTexturedRect(float n_width, float n_height, float n_centerX, float n_centerY, Texture* n_texture,float n_textureXScale, float n_textureYScale);
	static MeshTexture* GetTexturedRectProportionalByWidth(float n_width,float n_centerX, float n_centerY, Texture* n_texture, float n_textureXScale, float n_textureYScale);
	static MeshTexture* GetTexturedRectProportionalByHeight(float n_height,float n_centerX, float n_centerY, Texture* n_texture, float n_textureXScale, float n_textureYScale);

};

// a textured rec mesh for a texture
class MeshTexture : public Mesh
{
	float m_centerX,m_centerY,m_width,m_height,m_textureXScale,m_textureYScale;
public:

	MeshTexture(float n_width, float n_height, float n_centerX, float n_centerY, Texture* n_texture,float n_textureXScale, float n_textureYScale,float* n_vertices, float* n_colors, float* n_normals, float* n_textureCoords, unsigned int n_size);

	float GetWidth() { return m_width; }
	float GetHeight() { return m_height; }

};

class MeshManager
{
	std::map<std::string,Mesh*> m_map;
public:

	void AddMesh(std::string n_name, Mesh* n_mesh);
	Mesh* GetMesh(std::string n_name);
};

extern MeshManager g_meshManager;


#endif