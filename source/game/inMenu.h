#ifndef _INMENU_H_
#define _INMENU_H_

#include "../engine/engine.h"

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