#ifndef _INSCENENODE_H_
#define _INSCENENODE_H_

#include "../engine/engine.h"

class SceneNode_Test : public SceneNode
{
	float m_width, m_height;
	AnimationManager m_animationManager;
public:
	SceneNode_Test(unsigned int n_id, float n_width, float n_height);
	virtual void VRender( double n_interpolation, glm::mat4x4& n_matrix );
	virtual void VUpdate();
};

#endif
