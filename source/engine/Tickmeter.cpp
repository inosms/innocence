#include "Tickmeter.h"
#include <SDL2/SDL.h>
#include <iostream>

Tickmeter::Tickmeter(std::string n_name) : 
	m_lastSecond(SDL_GetTicks()),
	m_ticksUntilLastSecond(0),
	m_name(n_name)
	{}

void Tickmeter::Measure()
{
	unsigned int tmp_nowTime = SDL_GetTicks();
	if( tmp_nowTime - m_lastSecond > 1000 )
	{
		std::cout << "|| " << m_name << " " << m_ticksUntilLastSecond << " TPS " << std::endl;
		m_ticksUntilLastSecond = 0;
		m_lastSecond = tmp_nowTime;
	}

	m_ticksUntilLastSecond++;
}