#include "inMenu.h"
#include "inHumanView.h"

Menu_MainMenu::Menu_MainMenu() : Menu()
{

}

void Menu_MainMenu::VUpdate()
{
}

void Menu_MainMenu::VRender( double n_interpolation )
{
	glActiveTexture( GL_TEXTURE0 );

	static Texture* tmp_texture = new Texture("./test.png");

	tmp_texture->Bind();

	float n_vert[] = {0.f,0.f,0.f,
					  1.f,0.f,0.f,
					  1.f,1.f,0.f,
					  1.f,1.f,0.f,
					  0.f,1.f,0.f,
					  0.f,0.f,0.f};
	float n_tex[] = {0.f,0.f,
					 1,0.f,
					 1,1,
					 1,1,
					 0.f,1,
					 0.f,0.f};

	static Mesh* tmp_mesh = new Mesh(n_vert,nullptr,nullptr,n_tex,18);

	test->SetMat("modelview",glm::mat4x4());
	test->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));
	test->SetMat("normalMat", glm::transpose(glm::inverse(glm::mat4x4())));
	test->SetTexture("tex",0);


	tmp_mesh->Render();
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