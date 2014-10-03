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

class Keyframes
{
	struct Keyframe
	{
		glm::mat4 matrix;
		double time;
	};

	std::vector<Keyframe> m_keyframes;
public:
	void AddKeyframe(glm::mat4 n_keyframeMatrix, double n_time);

	// returns an interpolated matrix for the given time
	glm::mat4 GetAnimationMatrix(double n_time);
};

class Animatable
{
protected:
	// used to identify the Animatable object
	// must be unique!
	std::string m_identifier = "";
public:
	void SetIdentifier(std::string n_identifier);
	bool IsIdentifier(std::string n_identifier);

	// this is called every tick with an updated animation matrix according
	// to the keyframes
	virtual void VSetAnimationMatrix(glm::mat4 n_mat) = 0;
	virtual Animatable* VFindAnimatable(std::string n_identifier) = 0;
};

class Bone : public Animatable
{

};

class Animation
{
	std::map<Animatable*,Keyframes> m_keyframes;

	// after this time the animation is reset and repeated
	double m_duration;

	double m_time = 0;
public:
	Animation(std::string n_name,double n_duration);
	std::string m_name;
	// increments the internal time value
	void Tick(unsigned int n_ticksPerSecond);
	void AddKeyframesForAnimatable(Animatable* n_animatable,Keyframes n_keyframes);

	// applies the current animation matrix to the Animatable* objects
	// and also the new next animation matrix for interpolation TODO:!!
	void Apply(double n_interpolation);
};

// for each handle of a root mesh node there can exist
// a seperate AnimationManager, which manages the animation
// seperately for each instance!
// in memory there is always only one mesh object, but multiple
// animation manager, which each define a seperate animation
class AnimationManager
{
	// a vector of all possible animations;
	std::vector<Animation> m_animations;

	// the index of the active animation
	// -1 if no active animation
	int m_activeAnimation = -1;
public:
	void AddAnimation(Animation n_animation);
	void SetActiveAnimation(std::string n_name);

	// Call this in the update method
	// this ticks the current active animation one step further
	void Tick();

	// call this in the render method to apply the
	// animation to the mesh
	void Apply(double n_interpolation);
};

class MeshNode : public Animatable
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

	// the meshes which are used for rendering
	std::vector<Mesh*> m_meshes;

	// this containes all the animations for this mesh
	// for each scene node there must exist a seperate copy of this
	// otherwise all animations would be tied together (which is not really
	// reasonable)
	AnimationManager m_animationManager;

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

	// for modifying this manager
	AnimationManager& GetAnimationManagerReference();

	// for generating a copy
	// whenever you want to render something
	// YOU HAVE TO GET A COPY OF THIS!
	// then first Apply the animation and then render!
	// otherwise (if not applied) the mesh will have the last animation
	// state, which might be anything
	// if the mesh is not animated this is not needed, as it never changes
	AnimationManager GetAnimationManagerCopy();

	// inherited from Animatable
	// this is called when the animation ticks
	void VSetAnimationMatrix(glm::mat4 n_mat);
	Animatable* VFindAnimatable(std::string n_identifier);
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
