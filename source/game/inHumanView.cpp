#include "inHumanView.h"
#include "inHumanViewListener.h"
#include "inGameObject.h"
#include "inMenu.h"
#include "inController.h"

FBO* g_fbo;

inHumanView::inHumanView() : GameView_Human( new inHumanViewListener(this) )
{
	GetEventManager()->SendEvent( std::shared_ptr<Event_AddScreenLayer>(new Event_AddScreenLayer(new Menu_Start())));
	m_controller = new inController();
}

void inHumanView::VInit()
{
	GameView_Human::VInit();
	g_meshManager.AddMesh( "monkey" , new Mesh(GetResourcePath("monkey.dae")));
	g_fbo = new FBO();
	// for color
	g_fbo->AddColorTexture();
	// for normals
	g_fbo->AddColorTexture();
	// depth
	g_fbo->AddDepthTexture();

	g_fbo->CheckState();

	Shader* tmp_shader =new Shader(GetResourcePath("effect_test_vertex.shader"),GetResourcePath(""),GetResourcePath("effect_test_fragment.shader"));
	tmp_shader->Init();
	Effect* tmp_testEffect;
	tmp_testEffect = new Effect(tmp_shader);
	tmp_testEffect->AddInputTexture("texture_screen",g_fbo->GetTexture(0));
	tmp_testEffect->AddInputTexture("texture_screen_normal",g_fbo->GetTexture(1));
	m_effectManager.AddEffect("test",tmp_testEffect);
/*
	Shader* tmp_shader2 =new Shader(GetResourcePath("effect_test_vertex.shader"),GetResourcePath(""),GetResourcePath("effect_test2_fragment.shader"));
	tmp_shader2->Init();
	Effect* tmp_testEffect2;
	tmp_testEffect2 = new Effect(tmp_shader2);
	tmp_testEffect2->AddInputTexture("texture_lastEffect",tmp_testEffect->GetResultTexture());
	m_effectManager.AddEffect("second",tmp_testEffect2);
*/
}

void inHumanView::VRender(double n_interpolation)
{
	g_fbo->Bind();
	GameView_Human::VRender(n_interpolation);
	g_fbo->UnBind();

	m_effectManager.ApplyAndRenderResult();
}
