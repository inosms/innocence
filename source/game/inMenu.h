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

class Menu_MainMenu : public Menu
{
	Mesh* m_textureMesh;
	Texture* m_text;
	Mesh* m_textMesh;
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