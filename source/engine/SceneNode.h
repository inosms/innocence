#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include <vector>
#include "Math.h"
#include "Camera.h"

class SceneNode
{
protected:

	// this is the same id as the connected game object
	unsigned int m_id;

	// marks if this node has alpha
	// if it has renderering this node requires 
	// renderering everything else before this node
	// and then sorting with other alpha nodes 
	// and then finally rendering
	bool m_hasAlpha = false;

	glm::mat4x4 m_matrix;

	// this just saves the old matrix if 
	// SetNewMatrix is called
	// this is used for interpolation calculation
	glm::mat4x4 m_oldMat;

	std::vector<SceneNode*> m_children;

	// renders this and children 
	// BUT only if they are not marked as alpha nodes
	// matrix holds the parents translation
	// if a scene node has alpha it is added to the
	// list with the parent transformation matrix
	void RecursiveRenderCall_NonAlpha(double n_interpolation, glm::mat4x4& n_matrix, std::vector<std::pair<SceneNode*,glm::mat4x4> >& n_alphaNodes);

	// renders this and children (also alphas)
	// matrix holds the parents transformation
	void RecursiveRenderCall_All(double n_interpolation, glm::mat4x4& n_matrix);

public:

	SceneNode(unsigned int n_id);

	// render call with intpolation value and current transformation matrix
	// this matrix should be applied if the parents translations and
	// transformations are wished to be applied
	// the matrix basicall is:
	// parent transformation matrix * this matrix
	// those two are not purely virtual as this class
	// may serve as a root node
	virtual void VRender( double n_interpolation, glm::mat4x4& n_matrix ){}
	virtual void VUpdate(){}

	void SetNewMatrix( glm::mat4x4 n_mat );

	bool HasAlpha(); 
	void SetAlpha( bool n_alpha );

	// renders this and non alpha children first; 
	// then renders all alpha children sorted
	// therefore it should only be called by root node
	// it requires a camera object which is applied 
	// before rendering
	void RecursiveRenderCall_TopLevel(double n_interpolation,Camera& n_camera);
	void RecursiveDeleteCall(unsigned int n_id);

	// recursively updates this node and all children
	void RecursiveUpdateCall();

	SceneNode* RecursiveSearch( unsigned int n_id);
	void AddChild(SceneNode* n_node);

	virtual ~SceneNode(){}

};

#endif