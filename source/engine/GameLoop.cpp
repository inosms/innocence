#include "GameLoop.h"
#include "Error.h"
#include "Tickmeter.h"
#include <SDL2/SDL.h>

GameLoop::GameLoop()  : m_running( true ), 
						m_gameLogicTicksDelay(25), 
						m_lastGameLogicTickTime(0),
						m_maxFrameSkips(3)
{

}

int GameLoop::Run()
{
	if( VInit() == false )
	{
		ERROR_MESSAGE("An Error occured during Init Q.Q");
		return -1;
	}
	
	// Set the tick start time to now
	m_lastGameLogicTickTime = SDL_GetTicks() - m_gameLogicTicksDelay;

	// this variable counts the frame skips that can occur if 
	// the logic can't keep up; then the screen will have a framedrop,
	// but the logic should try to not freeze
	int tmp_frameSkips = 0;

	Tickmeter tmp_tickmeter("GameLoop");
	while( m_running )
	{
		tmp_tickmeter.Measure();
		
		// while ticks should be done; but the maximum number of frame skips is not completely used up
		// get input and udpate the logic
		while( SDL_GetTicks() >= m_gameLogicTicksDelay + m_lastGameLogicTickTime && tmp_frameSkips < m_maxFrameSkips )
		{
			PEDANTIC_DEBUG_MESSAGE("Logic Update with " << tmp_frameSkips << " current frameSkips");
			VInput();
			VUpdate();

			m_lastGameLogicTickTime += m_gameLogicTicksDelay;
			tmp_frameSkips++;
		}

		// reset the frameskips
		tmp_frameSkips = 0;
		
		// interpolation is a value between 0 and 1 which interpolates between the gamelogic updates
		double tmp_interpolation = (SDL_GetTicks() - m_lastGameLogicTickTime)/((double) m_gameLogicTicksDelay);

		VRender(tmp_interpolation);

		if( m_lastGameLogicTickTime >= 1000 )
			m_running = false;
	}

	if( !VExit() ) return -1;
	else return 0;
}