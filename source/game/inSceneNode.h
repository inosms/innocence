#ifndef _INSCENENODE_H_
#define _INSCENENODE_H_

#include "../engine/engine.h"

class SceneNode_Test : public SceneNode
{
	int m_width;
public:
	SceneNode_Test(unsigned int n_id, int n_width);
	virtual void VRender( double n_interpolation, glm::mat4x4& n_matrix );
};

#endif
