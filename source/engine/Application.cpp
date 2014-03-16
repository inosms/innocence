#include "Application.h"
#include "Error.h"
#include "VideoSystem_SDL_OpenGL.h"

Application::Application()
{
	m_videoSystem = new VideoSystem_SDL_OpenGL();
}

void Application::VUpdate()
{
	PEDANTIC_DEBUG_MESSAGE("VUpdate() called");

	// TODO 
}

void Application::VRender(double n_interpolation)
{
	PEDANTIC_DEBUG_MESSAGE("VRender(double) with " << n_interpolation << " as interpolation");

	m_videoSystem->VClearScreen();

	// TODO 

	m_videoSystem->VUpdateScreen();
}

void Application::VInput()
{
	// TODO 
}

bool Application::VInit()
{
	if( !m_videoSystem->VInit() ) return false;

	// TODO 
	
	return true;
}

bool Application::VExit()
{
	if( !m_videoSystem->VExit() ) return false;

	// TODO 

	DEBUG_MESSAGE("clean exit");
	return true;
}