#include "inHumanView.h"
#include "inHumanViewListener.h"
#include "inGameObject.h"
#include "inMenu.h"

inHumanView::inHumanView() : GameView_Human( new inHumanViewListener(this) )
{
	GetEventManager()->SendEvent( std::shared_ptr<Event_AddScreenLayer>(new Event_AddScreenLayer(new Menu_MainMenu())));
}

Shader* test = nullptr;

void inHumanView::VInit()
{
	GameView_Human::VInit();
	if(!test)
	{
		test = new Shader("../source/game/vertex.shader","","../source/game/fragment.shader");
		test->Init();
	}
	g_meshManager.AddMesh( "monkey" , new Mesh("../source/game/untitled.obj"));
}
void inHumanView::VRender(double n_interpolation)
{
	test->Begin();
		GameView_Human::VRender(n_interpolation);
		g_meshManager.GetMesh("monkey")->Render();
	test->End();
}