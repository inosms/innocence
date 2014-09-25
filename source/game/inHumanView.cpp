#include "inHumanView.h"
#include "inHumanViewListener.h"
#include "inGameObject.h"
#include "inMenu.h"
#include "inController.h"

inHumanView::inHumanView() : GameView_Human( new inHumanViewListener(this) )
{
	GetEventManager()->SendEvent( std::shared_ptr<Event_AddScreenLayer>(new Event_AddScreenLayer(new Menu_Start())));
	m_controller = new inController();
}

void inHumanView::VInit()
{
	GameView_Human::VInit();
	g_meshManager.AddMesh( "monkey" , new Mesh("../source/game/untitled.obj"));
}
