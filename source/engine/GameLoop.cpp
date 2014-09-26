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
	VInit();

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
		double tmp_interpolation = 0;
		if( m_calculateInterpolation ) tmp_interpolation = (SDL_GetTicks() - m_realLastGameLogicTickTime)/((double) m_gameLogicTicksDelay);

		// prevents interpolation form exploding
		tmp_interpolation = fmin(m_maxInterpolationValue,tmp_interpolation);
		VRender(tmp_interpolation);
	}

	// end the main game logic thread
	m_gameLogicThread.VOnRequestKill();

	VExit();
	return 0;
}

void GameLoop::Kill()
{
	m_running = false;
}

void GameLoop::SetInterpolationEnabled(bool n_enabled)
{
	m_calculateInterpolation = n_enabled;
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

		g_application->m_realLastGameLogicTickTime = g_application->m_lastGameLogicTickTime;

		Tickmeter tmp_tickmeterInner("GameLoop Logic inner loop");
		Tickmeter tmp_tickmeterOuter("GameLoop Logic outer loop");

		int tmp_tooSlowCounter = 0;
		while( m_running )
		{
			tmp_tickmeterOuter.Measure();
			tmp_tooSlowCounter = 0;
			while( m_running && g_application->m_lastGameLogicTickTime + g_application->m_gameLogicTicksDelay < SDL_GetTicks() )
			{
				tmp_tickmeterInner.Measure();

				g_application->VLogicUpdate();
				g_application->m_lastGameLogicTickTime += g_application->m_gameLogicTicksDelay;
				// set the current time for correct interpolation
				g_application->m_realLastGameLogicTickTime = SDL_GetTicks();

				// to prevent it from slowing down and then suddenly: ULTRASPEED
				// to do this: the lastGameLogicTickTime must be forwarded some steps
				if(tmp_tooSlowCounter++>5)
				{
					WARNING_MESSAGE("too slow for " << tmp_tooSlowCounter << " ticks! " << "time diff " <<	g_application->m_realLastGameLogicTickTime - 	g_application->m_lastGameLogicTickTime << " will now reset");

					unsigned int tmp_diff = g_application->m_realLastGameLogicTickTime - g_application->m_lastGameLogicTickTime;
					int tmp_times = tmp_diff/g_application->m_gameLogicTicksDelay;
					g_application->m_lastGameLogicTickTime += tmp_times * g_application->m_gameLogicTicksDelay;
				}
			}
		}
}
