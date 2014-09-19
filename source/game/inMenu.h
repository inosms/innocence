#ifndef _INMENU_H_
#define _INMENU_H_

#include "../engine/engine.h"

class Menu_Start : public Menu
{
	Mesh* m_textureMesh;
	int m_counter = 0;
	int m_maxCounter = 110;
public:
	Menu_Start();

	virtual void VUpdate();
	virtual void VRender( double n_interpolation );
	virtual bool VOnEvent(Event& n_event);
};

class Stripe
{
	glm::mat4 m_firstPos;
	glm::mat4 m_lastPos;
	// 0 = first pos; 1 = last pos
	float m_interpolationValue = 0;
	bool m_isAnimating = false;
	const float m_animationSpeed = 0.2;

	std::vector<MeshTexture*> m_texturedMeshes;
	Mesh* m_backgroundMesh;
	float m_textureOffset = 0.0f;
	float m_textureOffsetSpeed = 0.002;

public:
	void SetStartPos(glm::mat4 n_mat);
	void SetEndPos(glm::mat4 n_mat);
	Stripe();
	void Render(double n_interpolation);
	void Update();
	void AddTexturedMesh(MeshTexture* n_mesh);
	void StartAnimation();
};

class Menu_Title : public Menu
{
	Mesh* m_meshLayer0;
	Mesh* m_meshLayer1;
	glm::vec3 m_backgroundColor;
	ColorInterpolator m_interpolator;

	Stripe m_stripe1;
public:
	Menu_Title();

	virtual void VUpdate();
	virtual void VRender( double n_interpolation );
	virtual bool VOnEvent(Event& n_event);
	// called at the beginning and everytime the screen ratio
	// changes as the rotation angles depend 
	void SetStripeMatrices();
};

#endif