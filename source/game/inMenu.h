#ifndef _INMENU_H_
#define _INMENU_H_

#include "../engine/engine.h"

class Menu_Start : public Menu
{
	Mesh* m_textureMesh;
	// interal counter used for fading out
	int m_counter = 0;

	// how long the menu is visible
	int m_shownTime = 10;

	// fading out duration
	int m_fadeOutTime = 30;
public:
	Menu_Start();

	virtual void VUpdate();
	virtual void VRender( double n_interpolation );
	virtual bool VOnEvent(Event& n_event);
};

class Stripe;
class MenuItem
{
	std::string m_name;

	// the hint is shown if this is active
	// whith the m_hintDisplayer Menu Item
	// (the first item of this is removed (it assumes only one item)
	// and this hint is shown)
	std::string m_hint = "";
	MeshTexture* m_hintTexture = nullptr;
	Stripe* m_hintDisplayer = nullptr;

	MeshTexture* m_texture;

	// x position;
	float m_x = 0;

	// if currently selected
	bool m_active = false;
public:
	MenuItem(std::string n_name);
	MenuItem(std::string n_name, std::string n_hint, Stripe* n_hintDisplayer);
	MenuItem(MeshTexture* n_mesh, std::string n_name);
	~MenuItem();
	float GetWidth();
	float GetXPos();
	void SetXPos(float n_x);
	std::string GetName();
	void Render();
	// x and y not in pixel but in this ratio thingy
	bool CheckForCollision(float n_x, float n_y);

	bool IsActive();
	void SetActive(bool n_active);
};

class Stripe
{
	glm::mat4 m_firstPos;
	glm::mat4 m_lastPos;
	// 0 = first pos; 1 = last pos
	float m_interpolationValue = 0;
	bool m_isAnimating = false;
	bool m_hasAnimated = false;
	const float m_animationSpeed = 0.2;

	std::vector<MenuItem*> m_menuItems;
	MeshTexture* m_backgroundMesh;
	float m_textureOffset = 0.0f;
	float m_textureOffsetSpeed = 0.0008;

	const std::string m_seperator = " + ";

public:
	void SetStartPos(glm::mat4 n_mat);
	void SetEndPos(glm::mat4 n_mat);
	Stripe();
	void Render(double n_interpolation);
	void Update();

	void AddMenuItem(std::string n_menuItem);
	void AddMenuItem(std::string n_name, std::string n_hint, Stripe* n_displayer);
	void AddMenuItem(MenuItem* n_item);
	void AddMenuSeperator();

	void RemoveFirst();

	// call this whenever a new pos is added/removed/ect
	// this will update all the positions
	void RefreshMenuItemPositions();
	void StartAnimation();
//	void CheckForCollision();

	std::string GetActive();
	void OnKeyRight();
	void OnKeyLeft();
};

class Menu_Title : public Menu
{
	Mesh* m_meshLayer0;
	float m_backgroundOffset = 0;

	Mesh* m_meshLayer1;
	glm::vec3 m_backgroundColor;
	ColorInterpolator m_interpolator;

	Stripe m_stripe1;
	Stripe m_stripe2;
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
