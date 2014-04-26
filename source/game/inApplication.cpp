#include "inApplication.h"
#include "inProcesses.h"

GameLogic* inApplication::VCreateGameLogic()
{
	GetProcessManager()->AddProcess( new test_thread_process() );


	g_application->AddGameView( new GameView_Human() );
	// TODO THIS IS A DUMMY LOGIC!! (Well it's the basic one Q.Q)
	return new GameLogic();
}
