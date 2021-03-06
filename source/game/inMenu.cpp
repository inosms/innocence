#include "inMenu.h"
#include "inHumanView.h"
#include <cmath>

Shader* g_menuShader = nullptr;

Menu_Start::Menu_Start() : Menu()
{
	// first load the shader which will be used by all menus
	g_menuShader = g_shaderManager.AddShader("menu");

	g_textureManager.AddTexture("start_menu.png");

	// a fullscreenRect
	m_textureMesh = Mesh::GetTexturedRect(1,1,0.5,0.5,g_textureManager.GetTexture("start_menu.png"));
}

void Menu_Start::VUpdate()
{
	if( m_counter++ > m_shownTime + m_fadeOutTime) GoToNextScreenLayer(new Menu_Title());
}

void Menu_Start::VRender( double n_interpolation )
{
	g_menuShader->Begin();
	g_menuShader->SetTexture("tex",0);
	if( m_counter >= m_shownTime )
	g_menuShader->SetFloat("texture_multiplier",1.f-(float(m_counter-m_shownTime)/(m_fadeOutTime)));

	g_menuShader->SetMat("modelview",glm::mat4x4());
	g_menuShader->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));

	m_textureMesh->Render();
	g_menuShader->SetFloat("texture_multiplier",1.f);
	g_menuShader->End();
}

bool Menu_Start::VOnEvent(Event& n_event)
{
	return false;
}

MenuItem::MenuItem(std::string n_name)
{
	m_name = n_name;
	const float HEIGHT = 0.09f;
	Font* tmp_font = g_fontManager.GetFont("TravelingTypewriter.ttf");
	Texture* tmp_text = tmp_font->GetTextureForText(n_name,{ 255, 255, 255 });
	m_texture = Mesh::GetTexturedRectProportionalByHeight(HEIGHT,0,0,tmp_text,1,1);
	g_textureManager.AddTexture(n_name,tmp_text);
}

MenuItem::MenuItem(MeshTexture* n_mesh, std::string n_name)
{
	m_name = n_name;
	m_texture = n_mesh;
}

MenuItem::MenuItem(std::string n_name, std::string n_hint, Stripe* n_hintDisplayer) : MenuItem(n_name)
{
	m_hint = n_hint;
	m_hintDisplayer = n_hintDisplayer;
	const float HEIGHT = 0.05f;
	Font* tmp_font = g_fontManager.GetFont("TravelingTypewriter.ttf");
	Texture* tmp_text = tmp_font->GetTextureForText(n_hint,{ 255, 255, 255 });
	m_hintTexture = Mesh::GetTexturedRectProportionalByHeight(HEIGHT,0,0,tmp_text,1,1);
	g_textureManager.AddTexture(n_hint,tmp_text);

}

float MenuItem::GetWidth(){return m_texture->GetWidth();}
float MenuItem::GetXPos(){return m_x;}
void MenuItem::SetXPos(float n_x){m_x = n_x;}
std::string MenuItem::GetName(){return m_name;}
bool MenuItem::IsActive(){return m_active;}
void MenuItem::SetActive(bool n_active)
{
	m_active = n_active;

	if(IsActive() && m_hintDisplayer && m_hint != "" )
	{
		m_hintDisplayer->RemoveFirst();
		m_hintDisplayer->AddMenuItem( new MenuItem(m_hintTexture,m_hint) );
	}
}

bool MenuItem::CheckForCollision(float n_x, float n_y)
{
	// TODO
	return false;
}

MenuItem::~MenuItem()
{
	// if this holds all the textures remove them
	if( m_hintDisplayer )
	{
		g_textureManager.RemoveTexture(m_hint);
		g_textureManager.RemoveTexture(m_name);
	}

}


Stripe::Stripe() :
	m_firstPos(glm::mat4()),
	m_lastPos(glm::mat4())
{
	g_textureManager.AddTexture("stripe.png");
	// a fullscreenRect
	m_backgroundMesh = Mesh::GetTexturedRectProportionalByHeight(0.15f,0,0,g_textureManager.GetTexture("stripe.png"),1,1);
}


void Stripe::SetStartPos(glm::mat4 n_mat)
{
	m_firstPos = n_mat;
}

void Stripe::SetEndPos(glm::mat4 n_mat)
{
	m_lastPos = n_mat;
}

void Stripe::Update()
{
	m_textureOffset += m_textureOffsetSpeed;
	if( m_isAnimating ) m_interpolationValue += m_animationSpeed;
	if( m_interpolationValue >= 1 && m_isAnimating )
	{
		m_interpolationValue = 1;

		// delete the first element ( this should be the "PRESS ANY KEY" text)
		RemoveFirst();

		m_isAnimating = false;
		m_hasAnimated = true;

		m_textureOffsetSpeed = 0;
		m_textureOffset = 0;

		RefreshMenuItemPositions();
	}
}

void Stripe::RemoveFirst()
{
	if( m_menuItems.size() >= 1)
	{
		delete m_menuItems[0];
		m_menuItems.erase(m_menuItems.begin());
	}
}


void MenuItem::Render()
{
	m_texture->Render();
}


void Stripe::Render(double n_interpolation)
{
	float tmp_ratio = GetVideoSystem().VGetWidth()/float(GetVideoSystem().VGetHeight());
	g_menuShader->SetMat("projection",glm::ortho(0.f,tmp_ratio,0.f,1.f,-1.f,1.f));

	float tmp_interpolatedInterpolationValue = m_interpolationValue;
	if( m_isAnimating ) tmp_interpolatedInterpolationValue += n_interpolation * m_animationSpeed;
	glm::mat4 tmp_matDiff = m_lastPos - m_firstPos;
	glm::mat4 tmp_nowPos = m_firstPos + tmp_interpolatedInterpolationValue*tmp_matDiff;

	glm::mat4 tmp_backgroundTranslate = glm::translate(glm::mat4(), glm::vec3(m_backgroundMesh->GetWidth()/2.f,0,0));
	g_menuShader->SetMat("modelview",tmp_nowPos*tmp_backgroundTranslate);
	m_backgroundMesh->Render();

	g_menuShader->SetFloat("texture_u_offset",m_textureOffset+n_interpolation*m_textureOffsetSpeed);

	for( auto i_item : m_menuItems )
	{
		glm::mat4 tmp_translate = glm::translate(glm::mat4(1.f),glm::vec3(i_item->GetXPos(),0.f,0.f));

		if( m_isAnimating ) g_menuShader->SetVec4("texture_color",glm::vec4(1.f,1.f,1.f,1.f-m_interpolationValue));
		else g_menuShader->SetVec4("texture_color",glm::vec4(1.f,1.f,1.f,1.f));

		// white if active
		if( i_item->IsActive() )
			g_menuShader->SetVec4("texture_color",1.f,1.f,1.f,1.f);
		// black otherwise
		else
			g_menuShader->SetVec4("texture_color",0.f,0.f,0.f,1.f);



		g_menuShader->SetMat("modelview", tmp_nowPos * tmp_translate);
		i_item->Render();
		g_menuShader->SetVec4("texture_color",glm::vec4(1.f,1.f,1.f,1.f));
	}
	g_menuShader->SetFloat("texture_u_offset",0);
}

void Stripe::AddMenuItem(std::string n_menuItem)
{
	MenuItem* tmp_newMenuItem = new MenuItem(n_menuItem);
	AddMenuItem(tmp_newMenuItem);
}

void Stripe::AddMenuItem(MenuItem* n_item)
{
	m_menuItems.push_back(n_item);
	RefreshMenuItemPositions();
}

void Stripe::AddMenuItem(std::string n_name, std::string n_hint, Stripe* n_displayer)
{
	AddMenuItem(new MenuItem(n_name,n_hint,n_displayer));
}

void Stripe::AddMenuSeperator()
{
	AddMenuItem(m_seperator);
}

void Stripe::RefreshMenuItemPositions()
{
	float tmp_lastElementXPos = 0;
	for( auto i_item : m_menuItems )
	{
		i_item->SetXPos(tmp_lastElementXPos + i_item->GetWidth()/2.f);
		tmp_lastElementXPos += i_item->GetWidth();
	}
}

void Stripe::StartAnimation()
{
	if(m_hasAnimated) return;
	m_isAnimating = true;
}

std::string Stripe::GetActive()
{
	for( auto i_item : m_menuItems )
		if( i_item->IsActive() )
			return i_item->GetName();

	return "";
}


void Stripe::OnKeyRight()
{
	if(!m_hasAnimated) return;

	// none is active
	if(GetActive() == "" && m_menuItems.size() != 0)
	{
		m_menuItems[0]->SetActive(true);
	}
	else
	{
		for( int i = 0; i < m_menuItems.size(); i++ )
		{
			if( m_menuItems[i]->IsActive())
			{
				m_menuItems[i]->SetActive(false);
				m_menuItems[(i+1)%m_menuItems.size()]->SetActive(true);
				if(GetActive() != m_seperator ) return;
			}
		}
	}
}

void Stripe::OnKeyLeft()
{
	if(!m_hasAnimated) return;
	if(GetActive() == "" && m_menuItems.size() != 0)
	{
		m_menuItems[m_menuItems.size()-1]->SetActive(true);
	}
	else
	{
		for( int i = m_menuItems.size()-1; i >= 0; i-- )
		{
			if( m_menuItems[i]->IsActive())
			{
				m_menuItems[i]->SetActive(false);
				m_menuItems[((i-1)+m_menuItems.size())%m_menuItems.size()]->SetActive(true);
				if(GetActive() != m_seperator ) return;
			}
		}
	}
}

Menu_Title::Menu_Title() : m_backgroundColor()
{
	g_textureManager.AddTexture("title_menu_0.png");
	m_meshLayer0 = Mesh::GetTexturedRect(1,1,0.5,0.5,g_textureManager.GetTexture("title_menu_0.png"));
	g_textureManager.AddTexture("title_menu_1.png");
	m_meshLayer1 = Mesh::GetTexturedRect(1,1,0.5,0.5,g_textureManager.GetTexture("title_menu_1.png"));

	m_interpolator.Push(glm::vec4(0,175/255.f,240/255.f,1.f));
	m_interpolator.Push(glm::vec4(0,166/255.f,191/255.f,1.f));
	m_interpolator.Push(glm::vec4(129/255,185/255.f,215/255.f,1.f));

	g_fontManager.AddFont("TravelingTypewriter.ttf",255);

	std::string tmp_text = "";
	for( int i = 0; i < 2; i++ ) tmp_text += "PRESS ANY KEY + INNOCENCE + ";

	// add these right away as tmp_text is large enough no cover these up (ok hopefully...)
	m_stripe1.AddMenuItem(tmp_text);
	m_stripe2.AddMenuItem(tmp_text);


	m_stripe1.AddMenuItem("New Game","Starten Sie ein neues Spiel und klären Sie den Ayanokouji-Zwischenfall auf.",&m_stripe2);
	m_stripe1.AddMenuSeperator();
	m_stripe1.AddMenuItem("Load Game","Setzten Sie ein bereits begonnenes Spiel fort.",&m_stripe2);
	m_stripe1.AddMenuSeperator();
	m_stripe1.AddMenuItem("Options","Passen Sie Steuerung, Ton und Co. Ihren Bedürfnissen an.",&m_stripe2);
	m_stripe1.AddMenuSeperator();
	m_stripe1.AddMenuItem("Extras","Entdecken Sie Geheimnisse abseits der Handlung von Innocence.",&m_stripe2);

	SetStripeMatrices();
}

void Menu_Title::VUpdate()
{
	m_interpolator.Update(0.01);
	m_stripe1.Update();
	m_stripe2.Update();
	m_backgroundOffset+= 0.0001;
}
void Menu_Title::VRender( double n_interpolation )
{
	g_menuShader->Begin();
	g_menuShader->SetTexture("tex",0);
	g_menuShader->SetMat("modelview",glm::mat4x4());
	g_menuShader->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));

	g_menuShader->SetFloat("texture_u_offset",m_backgroundOffset);
	m_meshLayer0->Render();
	g_menuShader->SetFloat("texture_u_offset",0);


	g_menuShader->SetVec4("texture_color",m_interpolator.Get());
	m_meshLayer1->Render();
	g_menuShader->SetVec4("texture_color",1,1,1,1);

	m_stripe1.Render(n_interpolation);
	m_stripe2.Render(n_interpolation);
	g_menuShader->End();

}
bool Menu_Title::VOnEvent(Event& n_event)
{
	if( n_event.GetType() == Event_Type_Input_Key_Down )
	{
		// start animation; if already animated this will do nothing
		m_stripe1.StartAnimation();
		m_stripe2.StartAnimation();

		// handle specific keys:
		Event_Input_Key_Down& tmp_event = dynamic_cast<Event_Input_Key_Down&>(n_event);
		if( tmp_event.m_key == Event_Input_Key::D || tmp_event.m_key == Event_Input_Key::RIGHT )
		{
			m_stripe1.OnKeyRight();
		}
		else if( tmp_event.m_key == Event_Input_Key::A || tmp_event.m_key == Event_Input_Key::LEFT )
		{
			m_stripe1.OnKeyLeft();
		}
		else if( tmp_event.m_key == Event_Input_Key::RETURN && m_stripe1.GetActive() == "New Game")
		{
			RemoveThis();
			SEND_EVENT(Event_LoadLevel,"test.xml");
		}
		return true;
	}
	else if( n_event.GetType() == Event_Type_Input_Mousebutton_Down )
	{
		// TODO: mouse input?
	}
	else if( n_event.GetType() == Event_Type_WindowResize )
	{
		SetStripeMatrices();
	}
	return false;
}

void Menu_Title::SetStripeMatrices()
{
	// the ratio is important in order to compute the angle
	// ( as the stripe should go from one corner to the other)
	float tmp_ratio = GetVideoSystem().VGetWidth()/float(GetVideoSystem().VGetHeight());

	// set first stripe
	glm::mat4 tmp_1translate = glm::translate(glm::mat4(),glm::vec3(0,1.f,0));
	glm::mat4 tmp_1rotate = glm::rotate(glm::mat4x4(),-float(atan2(1,tmp_ratio)),glm::vec3(0,0,1));
	m_stripe1.SetStartPos(tmp_1translate*tmp_1rotate);
	glm::mat4 tmp_1endRotate = glm::rotate(glm::mat4(),float(2*PI)*5.f/360,glm::vec3(0,0,1));
	m_stripe1.SetEndPos(glm::translate(glm::mat4(),glm::vec3(0.f,0.5f,0.f))*tmp_1endRotate);

	// set second stripe
	glm::mat4 tmp_2rotate = glm::rotate(glm::mat4x4(),float(atan2(1,tmp_ratio)),glm::vec3(0,0,1));
	m_stripe2.SetStartPos(tmp_2rotate);
	glm::mat4 tmp_2endRotate = glm::rotate(glm::mat4(),float(2*PI)*-5.f/360,glm::vec3(0,0,1));
	m_stripe2.SetEndPos(glm::translate(glm::mat4(),glm::vec3(0.f,0.3f,0.f))*tmp_2endRotate);
}
