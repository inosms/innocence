#include "inMenu.h"
#include "inHumanView.h"
#include <cmath>

Menu_Start::Menu_Start() : Menu()
{
	g_textureManager.AddTexture("start_menu.png");

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

void Menu_Start::VUpdate()
{
	if( m_counter++ > m_maxCounter ) GoToNextScreenLayer(new Menu_MainMenu());
}

void Menu_Start::VRender( double n_interpolation )
{
	Texture* tmp_menuTexture = g_textureManager.GetTexture("start_menu.png");
	tmp_menuTexture->Bind(0);
	test->SetTexture("tex",0);
	if( m_counter >= 100 )
	test->SetFloat("texture_multiplier",1.f-(float(m_counter-100)/(m_maxCounter-100)));

	test->SetMat("modelview",glm::mat4x4());
	test->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));
	test->SetMat("normalMat", glm::transpose(glm::inverse(glm::mat4x4())));

	m_textureMesh->Render();
	test->SetFloat("texture_multiplier",1.f);
}

bool Menu_Start::VOnEvent(Event_Input& n_event)
{
	return false;
}


Menu_MainMenu::Menu_MainMenu() : Menu()
{
	g_textureManager.AddTexture("menu.png");
	g_fontManager.AddFont("TravelingTypewriter.ttf",255);
	SDL_Color textColor = { 255, 255, 255 };
	m_text = g_fontManager.GetFont("TravelingTypewriter.ttf")->GetTextureForText("test123",textColor);

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

	float tmp_newHeight = m_text->GetHeight()/float(m_text->GetWidth());
	float n_vert2[] = {0.f,0.f,0.f,
					  1.f,0.f,0.f,
					  1.f,tmp_newHeight,0.f,
					  1.f,tmp_newHeight,0.f,
					  0.f,tmp_newHeight,0.f,
					  0.f,0.f,0.f};
	float n_tex2[] = {0.f,1.f,
					 2,1.f,
					 2,0,
					 2,0,
					 0.f,0,
					 0.f,1.f};

	m_textMesh = new Mesh(n_vert2,nullptr,nullptr,n_tex2,18);
}

void Menu_MainMenu::VUpdate()
{
}

void Menu_MainMenu::VRender( double n_interpolation )
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	Texture* tmp_menuTexture = g_textureManager.GetTexture("menu.png");
	tmp_menuTexture->Bind(0);
	test->SetTexture("tex",0);
	test->SetMat("modelview",glm::mat4x4());
	test->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));
	test->SetMat("normalMat", glm::transpose(glm::inverse(glm::mat4x4())));
	m_textureMesh->Render();

	m_text->Bind(0);
	m_textMesh->Render();
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