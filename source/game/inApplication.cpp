#include "inApplication.h"
#include "inProcesses.h"

inApplication::inApplication() :
	Application()
	{}

GameLogic* inApplication::VCreateGameLogic()
{

	// TODO THIS IS A DUMMY LOGIC!! (Well it's the basic one Q.Q)
	return new GameLogic();
}
