#include "inApplication.h"
#include "inProcesses.h"
#include "inGameLogic.h"
#include "inHumanView.h"

inApplication::inApplication() :
	Application()
	{}

GameLogic* inApplication::VCreateGameLogic()
{
	GameView_Human* tmp_newView= new inHumanView();
	g_application->AddGameView(tmp_newView);

	return new inGameLogic();
}
