#include "inHumanView.h"
#include "inHumanViewListener.h"
#include "inGameObject.h"
#include "inMenu.h"
#include "inController.h"
#include "inEffect.h"

inHumanView::inHumanView() : GameView_Human( new inHumanViewListener(this) )
{
	GetEventManager()->SendEvent( std::shared_ptr<Event_AddScreenLayer>(new Event_AddScreenLayer(new Menu_Start())));
	m_controller = new inController();
}

void inHumanView::VInit()
{
	GameView_Human::VInit();
	g_meshNodeManager.AddMeshNode( "monkey" , Mesh::LoadMeshFromFile(GetResourcePath("monkey.dae")));

/*
	m_effectManager.AddEffect("blur_horizontal",new Effect_BlurHorizontal(g_fbo->GetTexture(1)));
	m_effectManager.AddEffect("blur_vertical",new Effect_BlurVertical(m_effectManager.GetEffect("blur_horizontal")->GetResultTexture()));
/*	m_effectManager.AddEffect("bloom",new Effect_Bloom(
											m_effectManager.GetEffect("blur_vertical")->GetResultTexture(),
											g_fbo->GetTexture(0)));
*/
}
