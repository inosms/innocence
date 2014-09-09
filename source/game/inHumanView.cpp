#include "inHumanView.h"
#include "inHumanViewListener.h"
#include "inGameObject.h"

inHumanView::inHumanView() : GameView_Human( new inHumanViewListener(this) )
{
	GetEventManager()->AddEventListener(m_listener,Event_Type_CreateNewObject);

	GetEventManager()->SendEvent( std::shared_ptr<Event_CreateNewObject>(new Event_CreateNewObject(new Creator_Test())));
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