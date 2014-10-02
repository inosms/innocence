#include "inHumanView.h"
#include "inHumanViewListener.h"
#include "inGameObject.h"
#include "inMenu.h"
#include "inController.h"
#include "inEffect.h"

FBO* g_fbo;

inHumanView::inHumanView() : GameView_Human( new inHumanViewListener(this) )
{
	GetEventManager()->SendEvent( std::shared_ptr<Event_AddScreenLayer>(new Event_AddScreenLayer(new Menu_Start())));
	m_controller = new inController();
}

void inHumanView::VInit()
{
	GameView_Human::VInit();
	g_meshNodeManager.AddMeshNode( "monkey" , Mesh::LoadMeshFromFile(GetResourcePath("monkey.dae")));
	g_fbo = new FBO();
	// for color
	g_fbo->AddColorTexture();
	// for normals
	g_fbo->AddColorTexture();
	// depth
	g_fbo->AddDepthTexture();

	g_fbo->CheckState();

	m_effectManager.AddEffect("blur_horizontal",new Effect_BlurHorizontal(g_fbo->GetTexture(1)));
	m_effectManager.AddEffect("blur_vertical",new Effect_BlurVertical(m_effectManager.GetEffect("blur_horizontal")->GetResultTexture()));
/*	m_effectManager.AddEffect("bloom",new Effect_Bloom(
											m_effectManager.GetEffect("blur_vertical")->GetResultTexture(),
											g_fbo->GetTexture(0)));

	m_effectManager.AddEffect("dof",new Effect_DOF(g_fbo->GetTexture(0),g_fbo->GetDepthTexture()));*/
}

void inHumanView::VRender(double n_interpolation)
{
	g_fbo->Bind();
	GameView_Human::VRender(n_interpolation);
	g_fbo->UnBind();

	m_effectManager.ApplyAndRenderResult();
}
