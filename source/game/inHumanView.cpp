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
	g_meshManager.AddMesh( "monkey" , new Mesh(GetResourcePath("monkey.obj")));
	g_fbo = new FBO();
	// for color
	g_fbo->AddColorTexture();
	// for normals
	g_fbo->AddColorTexture();
	// depth
	g_fbo->AddDepthTexture();

	g_fbo->CheckState();
}

void inHumanView::VRender(double n_interpolation)
{
	g_fbo->Bind();
	GameView_Human::VRender(n_interpolation);
	g_fbo->UnBind();

	GetVideoSystem()->VClearScreen();
	// render color texture
	g_fbo->Render(0);
	GetVideoSystem()->VUpdateScreen();

}
