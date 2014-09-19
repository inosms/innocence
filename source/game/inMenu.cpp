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

bool Menu_Start::VOnEvent(Event& n_event)
{
	return false;
}

Stripe::Stripe() : 
	m_firstPos(glm::mat4()),
	m_lastPos(glm::mat4())
{

	// TODO backgroundMesh
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
		if( m_texturedMeshes.size() >= 1)
		{
			Mesh* tmp_toDelete = m_texturedMeshes[0]; 
			m_texturedMeshes.erase(m_texturedMeshes.begin());
			delete tmp_toDelete;
		}
		m_isAnimating = false;

		m_textureOffsetSpeed = 0;
		m_textureOffset = 0;
	}
}

void Stripe::Render(double n_interpolation)
{
	float tmp_ratio = GetVideoSystem()->VGetWidth()/float(GetVideoSystem()->VGetHeight());
	g_menuShader->SetMat("projection",glm::ortho(0.f,tmp_ratio,0.f,1.f,-1.f,1.f));

	float tmp_interpolatedInterpolationValue = m_interpolationValue;
	if( m_isAnimating ) tmp_interpolatedInterpolationValue += n_interpolation * m_animationSpeed;
	glm::mat4 tmp_matDiff = m_lastPos - m_firstPos;
	glm::mat4 tmp_nowPos = m_firstPos + tmp_interpolatedInterpolationValue*tmp_matDiff;

	g_menuShader->SetMat("modelview",tmp_nowPos);
	//m_backgroundMesh->Render();

	g_menuShader->SetFloat("texture_u_offset",m_textureOffset+n_interpolation*m_textureOffsetSpeed);
	float lastElementEndPos = 0.0f;
	for( auto i_mesh : m_texturedMeshes )
	{
		float tmp_width = i_mesh->GetWidth();
		float tmp_thisRenderXPos = lastElementEndPos + tmp_width/2.f;
		lastElementEndPos += tmp_width;
		glm::mat4 tmp_translate = glm::translate(glm::mat4(1.f),glm::vec3(tmp_thisRenderXPos,0.f,0.f));

		if( m_isAnimating ) g_menuShader->SetVec4("texture_color",glm::vec4(1.f,1.f,1.f,1.f-m_interpolationValue));
		else g_menuShader->SetVec4("texture_color",glm::vec4(1.f,1.f,1.f,1.f));

		g_menuShader->SetMat("modelview", tmp_nowPos * tmp_translate);
		i_mesh->Render();
		g_menuShader->SetVec4("texture_color",glm::vec4(1.f,1.f,1.f,1.f));
	}
	g_menuShader->SetFloat("texture_u_offset",0);
	g_menuShader->End();

	//m_backgroundMesh->Render();

}

void Stripe::AddTexturedMesh(MeshTexture* n_mesh)
{
	m_texturedMeshes.push_back(n_mesh);
}

void Stripe::StartAnimation()
{
	m_isAnimating = true;
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
	
	{
		Texture* tmp_text = g_fontManager.GetFont("TravelingTypewriter.ttf")->GetTextureForText("PRESS ANY KEY +++ ",{ 0, 0, 0 });
		// this should really be enough even for large screens
		// furthermore this doesn't use up any more memory
		// as the texture is only stored once
		float tmp_width = 4; 
		float tmp_segmentsCount = 6; // how often the text is repeated
		float tmp_ratioHeightWidth = tmp_text->GetHeight()/float(tmp_text->GetWidth());
		float tmp_height = tmp_ratioHeightWidth*(tmp_width/tmp_segmentsCount);
		MeshTexture* tmp_textMesh = Mesh::GetTexturedRect(tmp_width,tmp_height,tmp_text,tmp_segmentsCount,1);
		m_stripe1.AddTexturedMesh(tmp_textMesh);
	}

	const float HEIGHT = 0.1f;
	Font* tmp_font = g_fontManager.GetFont("TravelingTypewriter.ttf");
	{
		Texture* tmp_text = tmp_font->GetTextureForText("New Game + ",{ 0, 0, 0 });
		MeshTexture* tmp_textMesh = Mesh::GetTexturedRectProportionalByHeight(HEIGHT,0,0,tmp_text,1,1);
		m_stripe1.AddTexturedMesh(tmp_textMesh);

		SetStripeMatrices();
	}
	{
		Texture* tmp_text = tmp_font->GetTextureForText("Load Game + ",{ 0, 0, 0 });
		MeshTexture* tmp_textMesh = Mesh::GetTexturedRectProportionalByHeight(HEIGHT,0,0,tmp_text,1,1);
		m_stripe1.AddTexturedMesh(tmp_textMesh);

		SetStripeMatrices();
	}
	{
		Texture* tmp_text = tmp_font->GetTextureForText("Options + ",{ 0, 0, 0 });
		MeshTexture* tmp_textMesh = Mesh::GetTexturedRectProportionalByHeight(HEIGHT,0,0,tmp_text,1,1);
		m_stripe1.AddTexturedMesh(tmp_textMesh);

		SetStripeMatrices();
	}
	{
		Texture* tmp_text = tmp_font->GetTextureForText("Extras",{ 0, 0, 0 });
		MeshTexture* tmp_textMesh = Mesh::GetTexturedRectProportionalByHeight(HEIGHT,0,0,tmp_text,1,1);
		m_stripe1.AddTexturedMesh(tmp_textMesh);

		SetStripeMatrices();
	}
	// TODO memory leak tmp_text
}

void Menu_Title::VUpdate()
{
	m_interpolator.Update(0.01);
	m_stripe1.Update();
}
void Menu_Title::VRender( double n_interpolation )
{
	g_menuShader->Begin();
	g_menuShader->SetTexture("tex",0);
	g_menuShader->SetMat("modelview",glm::mat4x4());
	g_menuShader->SetMat("projection",glm::ortho(0.f,1.f,0.f,1.f,-1.f,1.f));
	g_menuShader->SetMat("normalMat", glm::transpose(glm::inverse(glm::mat4x4())));	

	m_meshLayer0->Render();

	g_menuShader->SetVec4("texture_color",m_interpolator.Get());
	m_meshLayer1->Render();
	g_menuShader->SetVec4("texture_color",1,1,1,1);

	m_stripe1.Render(n_interpolation);
	g_menuShader->End();

}
bool Menu_Title::VOnEvent(Event& n_event)
{
	if( n_event.GetType() == Event_Type_Input_Key_Down )
	{
		//GoToNextScreenLayer(new Menu_MainMenu());
		m_stripe1.StartAnimation();
		return true;	
	}
	else if( n_event.GetType() == Event_Type_WindowResize )
	{
		SetStripeMatrices();
	}
	return false;
}

void Menu_Title::SetStripeMatrices()
{
	float tmp_ratio = GetVideoSystem()->VGetWidth()/float(GetVideoSystem()->VGetHeight());
	glm::mat4 tmp_1translate = glm::translate(glm::mat4(),glm::vec3(0,1.f,0));
	glm::mat4 tmp_1rotate = glm::rotate(glm::mat4x4(),-float(atan2(1,tmp_ratio)),glm::vec3(0,0,1));
	m_stripe1.SetStartPos(tmp_1translate*tmp_1rotate);
	m_stripe1.SetEndPos(glm::translate(glm::mat4(),glm::vec3(0.f,0.7f,0.f)));
}
