#ifndef _INMENU_H_
#define _INMENU_H_

#include "../engine/engine.h"

class Menu_Start : public Menu
{
	Mesh* m_textureMesh;
	int m_counter = 0;
	int m_maxCounter = 170;
public:
	Menu_Start();

	virtual void VUpdate();
	virtual void VRender( double n_interpolation );
	virtual bool VOnEvent(Event_Input& n_event);
};

class Menu_Title : public Menu
{
	Mesh* m_meshLayer0;
	Mesh* m_meshLayer1;
	glm::vec3 m_backgroundColor;
	ColorInterpolator m_interpolator;
	float m_textureOffset = 0;
	Mesh* m_meshLayer2;
	Mesh* m_meshLayer3;
public:
	Menu_Title();

	virtual void VUpdate();
	virtual void VRender( double n_interpolation );
	virtual bool VOnEvent(Event_Input& n_event);
};

class Menu_MainMenu : public Menu
{
	Mesh* m_textureMesh;
public:
	Menu_MainMenu();

	virtual void VUpdate();
	virtual void VRender( double n_interpolation );
	virtual bool VOnEvent(Event_Input& n_event);
};

class Menu_Test : public Menu
{
public:
	Menu_Test();
	
	virtual void VUpdate();
	virtual void VRender( double n_interpolation );
	virtual bool VOnEvent(Event_Input& n_event);
};

#endif