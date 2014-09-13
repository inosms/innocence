#include "inMenu.h"
#include "inHumanView.h"

Menu_MainMenu::Menu_MainMenu() : Menu()
{
	g_textureManager.AddTexture("menu.png");

	// a fullscreen quadrat
	float n_vert[] = {0.f,0.f,0.f,
					  1.f,0.f,0.f,
					  1.f,1.f,0.f,
					  1.f,1.f,0.f,
					  0.f,1.f,0.f,
					  0.f,0.f,0.f};
	float n_tex[] = {0.f,1.f,
					 1,1.f,
					 1,0,
					 1,0,
					 0.f,0,
					 0.f,1.f};

	m_textureMesh = new Mesh(n_vert,nullptr,nullptr,n_tex,18);
}

void Menu_MainMenu::VUpdate()
{
}

void Menu_MainMenu::VRender( double n_interpolation )
{
	Texture* tmp_menuTexture = g_textureManager.GetTexture("menu.png");
	tmp_menuTexture->Bind(0);
	test->SetTexture("tex",0);

	test->SetMat("modelview",glm::mat4x4());
	test->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));
	test->SetMat("normalMat", glm::transpose(glm::inverse(glm::mat4x4())));

	m_textureMesh->Render();
}

bool Menu_MainMenu::VOnEvent(Event_Input& n_event)
{
	if( n_event.GetType() == Event_Type_Input_Mousebutton_Down )
	{
		std::cout << "MOUSE BUTTON DOWN! GO TO NEXT SCREEN!" << std::endl;
		//sGoToNextScreenLayer(new Menu_Test());	
	}
	return false;
}


Menu_Test::Menu_Test() : Menu()
{

}

void Menu_Test::VUpdate()
{
	std::cout << "testmenu" << std::endl;
}
void Menu_Test::VRender( double n_interpolation )
{

}
bool Menu_Test::VOnEvent(Event_Input& n_event)
{
	return false;
}