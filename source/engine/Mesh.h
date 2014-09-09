#ifndef _MESH_H_
#define _MESH_H_

#include <string>
#include <map>

class Mesh
{
	// 4, because:
	// vertex, color, normal, texture
	unsigned int m_vbo[4];
	unsigned int m_vao;

	unsigned int m_verticesCount;
public:

	// init via obj file
	Mesh(std::string n_filename);

	// init via arrays
	Mesh( float* n_vertices, float* n_colors, float* n_normals, float* n_textureCoords, unsigned int n_size );

	void Render();
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