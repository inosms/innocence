#include "inMenu.h"
#include "inHumanView.h"
#include <cmath>


Shader* g_menuShader = nullptr;



Menu_Start::Menu_Start() : Menu()
{
	// first load the shader which will be used by all menus
	g_menuShader = g_shaderManager.AddShader("menushader","menu_vertex.shader","","menu_fragment.shader");

	g_textureManager.AddTexture("start_menu.png");

	// a fullscreenRect
	m_textureMesh = Mesh::GetTexturedRect(1,1,0.5,0.5,g_textureManager.GetTexture("start_menu.png"));
}

void Menu_Start::VUpdate()
{
	if( m_counter++ > m_maxCounter ) GoToNextScreenLayer(new Menu_Title());
}

void Menu_Start::VRender( double n_interpolation )
{
	g_menuShader->Begin();
	g_menuShader->SetTexture("tex",0);
	if( m_counter >= 100 )
	g_menuShader->SetFloat("texture_multiplier",1.f-(float(m_counter-100)/(m_maxCounter-100)));

	g_menuShader->SetMat("modelview",glm::mat4x4());
	g_menuShader->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));
	g_menuShader->SetMat("normalMat", glm::transpose(glm::inverse(glm::mat4x4())));

	m_textureMesh->Render();
	g_menuShader->SetFloat("texture_multiplier",1.f);
	g_menuShader->End();
}

bool Menu_Start::VOnEvent(Event_Input& n_event)
{
	return false;
}


Menu_Title::Menu_Title() : m_backgroundColor()
{
	g_textureManager.AddTexture("title_menu_0.png");
	m_meshLayer0 = Mesh::GetTexturedRect(1,1,0.5,0.5,g_textureManager.GetTexture("title_menu_0.png"));
	g_textureManager.AddTexture("title_menu_1.png");
	m_meshLayer1 = Mesh::GetTexturedRect(1,1,0.5,0.5,g_textureManager.GetTexture("title_menu_1.png"));

	m_interpolator.Push(glm::vec3(0,175/255.f,240/255.f));
	m_interpolator.Push(glm::vec3(0,166/255.f,191/255.f));
	m_interpolator.Push(glm::vec3(129/255,185/255.f,215/255.f));

	g_fontManager.AddFont("TravelingTypewriter.ttf",255);
	SDL_Color textColor = { 0, 0, 0 };
	Texture* tmp_text = g_fontManager.GetFont("TravelingTypewriter.ttf")->GetTextureForText("PRESS ANY KEY +++ ",textColor);

	// this should really be enough even for large screens
	// furthermore this doesn't use up any more memory
	// as the texture is only stored once
	float tmp_width = 5; 
	float tmp_segmentsCount = 8; // how often the text is repeated
	float tmp_ratioHeightWidth = tmp_text->GetHeight()/float(tmp_text->GetWidth());
	float tmp_height = tmp_ratioHeightWidth*(tmp_width/tmp_segmentsCount);
	m_meshLayer3 = Mesh::GetTexturedRect(tmp_width,tmp_height,tmp_text,tmp_segmentsCount,1);
	// TODO memory leak tmp_text
}

void Menu_Title::VUpdate()
{
	m_interpolator.Update(0.01);
	m_textureOffset += 0.002;
}
void Menu_Title::VRender( double n_interpolation )
{
	g_menuShader->Begin();
	g_menuShader->SetTexture("tex",0);
	g_menuShader->SetMat("modelview",glm::mat4x4());
	g_menuShader->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));
	g_menuShader->SetMat("normalMat", glm::transpose(glm::inverse(glm::mat4x4())));	

	g_menuShader->SetFloat("texture_u_offset",m_textureOffset*0.1);
	m_meshLayer0->Render();
	g_menuShader->SetFloat("texture_u_offset",1);

	g_menuShader->SetVec3("texture_color",m_interpolator.Get());
	m_meshLayer1->Render();
	g_menuShader->SetVec3("texture_color",1,1,1);

	float tmp_ratio = GetVideoSystem()->VGetWidth()/float(GetVideoSystem()->VGetHeight());
	g_menuShader->SetMat("projection",glm::ortho(0.f,tmp_ratio,0.f,1.f,-1.f,1.f));
	glm::mat4x4 tmp_rotate = glm::rotate(glm::mat4x4(),float(atan2(1,tmp_ratio)),glm::vec3(0,0,1));
	g_menuShader->SetMat("modelview",tmp_rotate);
	g_menuShader->SetFloat("texture_u_offset",m_textureOffset);
	m_meshLayer3->Render();
	g_menuShader->SetFloat("texture_u_offset",0);
	g_menuShader->End();

}
bool Menu_Title::VOnEvent(Event_Input& n_event)
{
	if( n_event.GetType() == Event_Type_Input_Key_Down )
	{
		//GoToNextScreenLayer(new Menu_MainMenu());
		return true;	
	}
	return false;
}


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
	g_menuShader->SetTexture("tex",0);
	g_menuShader->SetMat("modelview",glm::mat4x4());
	g_menuShader->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));
	g_menuShader->SetMat("normalMat", glm::transpose(glm::inverse(glm::mat4x4())));
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