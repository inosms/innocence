#ifndef _MESH_H_
#define _MESH_H_

#include <string>
#include <map>
#include <vector>
#include "Texture.h"
#include "Math.h"

class Shader;
class MeshTexture;
class MeshNode;
class Mesh
{
	// 4, because:
	// vertex, color, normal, texture
	unsigned int m_vbo[4];
	unsigned int m_vao;

	unsigned int m_verticesCount;

	Texture* m_texture = nullptr;

public:
	// init via arrays
	// vert/col/normal/tex are all per triangle
	// first 3 elements in vert/col/normal belong to 1 vert; first 2 in texCoord to 1
	// n_size is the number of vertices*3; thus elements in n_vertices/col/nor
	Mesh( float* n_vertices, float* n_colors, float* n_normals, float* n_textureCoords,std::vector<unsigned int> n_indices, unsigned int n_size );
	virtual ~Mesh();

	void Render();

	void SetTexture(Texture* n_texture);
	Texture* GetTexture();

	// loads a mesh from the given path and returns the root mesh node
	static MeshNode* LoadMeshFromFile(std::string n_filename);

	// returns just a rect width w&h=1 center=0,0,0 and white
	static Mesh* GetRect();
	static MeshTexture* GetTexturedRect(float n_width, float n_height, Texture* n_texture);
	static MeshTexture* GetTexturedRect(float n_width, float n_height, float n_centerX, float n_centerY, Texture* n_texture);
	static MeshTexture* GetTexturedRect(float n_width, float n_height, Texture* n_texture,float n_textureXScale, float n_textureYScale);
	static MeshTexture* GetTexturedRect(float n_width, float n_height, float n_centerX, float n_centerY, Texture* n_texture,float n_textureXScale, float n_textureYScale);
	static MeshTexture* GetTexturedRectProportionalByWidth(float n_width,float n_centerX, float n_centerY, Texture* n_texture, float n_textureXScale, float n_textureYScale);
	static MeshTexture* GetTexturedRectProportionalByHeight(float n_height,float n_centerX, float n_centerY, Texture* n_texture, float n_textureXScale, float n_textureYScale);

};

class MeshNode
{
protected:
	// a mesh can have a number of children which are then drawn
	// relative; this is very similar to a scene graph
	std::vector<MeshNode*> m_children;

	// the child Meshes only store a pointer to meshes in this list;
	// and only the root note stores the child meshes in this list!!!
	std::vector<Mesh*> m_rootMeshList;

	// matrix describing the relative transformation to the parent mesh
	glm::mat4 m_matrix = glm::mat4(1.f);

	// the mesh which is used for rendering
	// if nullptr, this node is root or parent of a node with many children
	std::vector<Mesh*> m_meshes;
public:
	MeshNode(glm::mat4 n_matrix);
	virtual ~MeshNode();

	// render the mesh recursively
	// the shader must provide a modelview matrix variable
	// also the shader must be bind and the perspective matrix must be set beforehand
	void Render(Shader* n_shader = nullptr, glm::mat4 n_relativeMat = glm::mat4(1.f));
	void AddChild(MeshNode* n_child);
	void AddMesh(Mesh* n_mesh);

	// adds the root mesh list; CALL THIS ONLY AT ROOT LEVEL!
	// the root will delete all the Meshes and child nodes then!
	// this is necessary as it is possible to have shared meshes for children!!
	void SetMeshRootList(std::vector<Mesh*> n_rootMeshList);
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

class MeshNodeManager
{
	std::map<std::string,MeshNode*> m_map;
public:

	void AddMeshNode(std::string n_name, MeshNode* n_mesh);
	MeshNode* GetMeshNode(std::string n_name);
};

extern MeshNodeManager g_meshNodeManager;


#endif
