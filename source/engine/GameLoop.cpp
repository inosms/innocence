#include "GameLoop.h"
#include "Error.h"
#include "Tickmeter.h"
#include "Process.h"
#include "Application.h"
#include <SDL2/SDL.h>

GameLoop::GameLoop()  : m_running( true ),
						m_gameLogicTicksDelay(25),
						m_lastGameLogicTickTime(0)
{

}

int GameLoop::Run()
{
	if( VInit() == false )
	{
		ERROR_MESSAGE("An Error occured during Init Q.Q");
		return -1;
	}

	// as the ticktime is always update by another thread; a simple !=
	// checks if an update has occured and updates this thread then
	// -1 because it updates at the beginning then. -1 is just a random number
	static unsigned int tmp_lastTickTime = m_lastGameLogicTickTime-1;

	m_gameLogicThread.VInitialize();

	Tickmeter tmp_tickmeter("GameLoop View");
	while( m_running )
	{
		tmp_tickmeter.Measure();

		if( tmp_lastTickTime != m_lastGameLogicTickTime )
		{
			PEDANTIC_DEBUG_MESSAGE("Logic Update with " << tmp_frameSkips << " current frameSkips");
			VInput();
			VViewUpdate();

			tmp_lastTickTime = m_lastGameLogicTickTime;
		}

		// interpolation is a value between 0 and 1 which interpolates between the gamelogic updates
		double tmp_interpolation = (SDL_GetTicks() - m_lastGameLogicTickTime)/((double) m_gameLogicTicksDelay);

		VRender(tmp_interpolation);
	}

	// end the main game logic thread
	m_gameLogicThread.VOnRequestKill();

	if( !VExit() ) return -1;
	else return 0;
}

void GameLoop::Kill()
{
	m_running = false;
}

unsigned int GameLoop::GetLastGameLogicTickTime()
{
	return m_lastGameLogicTickTime;
}

unsigned int GameLoop::GetGameLogicTicksDelay()
{
	return m_gameLogicTicksDelay;
}

void GameLogic_Thread::VThreadedMethod()
{
		// Set the tick start time to now
		g_application->m_lastGameLogicTickTime = SDL_GetTicks() - g_application->m_gameLogicTicksDelay;

		Tickmeter tmp_tickmeter("GameLoop Logic");

		while( m_running )
		{
			tmp_tickmeter.Measure();
			while( g_application->m_lastGameLogicTickTime + g_application->m_gameLogicTicksDelay < SDL_GetTicks() )
			{
				g_application->VLogicUpdate();
				g_application->m_lastGameLogicTickTime += g_application->m_gameLogicTicksDelay;

			}
		}
}
