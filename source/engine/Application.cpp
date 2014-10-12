#include "Application.h"
#include "Error.h"
#include "VideoSystem_SDL_OpenGL.h"

Application* g_application = nullptr ;

Application::Application()
{
	m_videoSystem = std::unique_ptr<VideoSystem_SDL_OpenGL>(new VideoSystem_SDL_OpenGL());

	g_application = this;
}

void Application::VRender(double n_interpolation)
{
	for( auto& i_gameView : m_gameViews )
		i_gameView->VRender(n_interpolation);
}

void Application::VViewUpdate()
{
	PEDANTIC_DEBUG_MESSAGE("VUpdate() called");

	for( auto& i_gameView : m_gameViews )
		i_gameView->VUpdate();
}


void Application::VLogicUpdate()
{
	m_processManager.Update();
	m_gameLogic->VUpdate();
}

void Application::VInput()
{
	m_videoSystem->VTranslateInput();
}

void Application::VInit()
{
	m_videoSystem->VInit();

	m_gameLogic = std::unique_ptr<GameLogic>(VCreateGameLogic());
	if( !m_gameLogic ) throw Exception("game logic must not be null");
}

void Application::VExit()
{
	// try to stop all processes
	m_processManager.KillAllProcesses();
	DEBUG_MESSAGE("Waiting for all processes to shut down");
		while( m_processManager.AllProcessesDead() == false )
		{
			m_processManager.Update();
		}
	DEBUG_MESSAGE("All processes shut down");
	m_gameLogic->Exit();

	m_videoSystem->VExit();

	DEBUG_MESSAGE("clean exit");
}

void Application::AddGameView(GameView* n_gameView)
{
	assert(n_gameView!=nullptr);
	n_gameView->VInit();
	m_gameViews.push_back(std::unique_ptr<GameView>(n_gameView));
}

void Application::RemoveGameView( unsigned int n_id )
{
	for( unsigned int i = 0; i < m_gameViews.size(); i++ )
	{
		if( m_gameViews[i]->GetID() == n_id )
		{
			m_gameViews.erase( m_gameViews.begin() +i );
			return;
		}
	}
}

UniqueNumberGenerator& GetUniqueNumberGenerator()
{
	return (g_application->m_uniqueNumberGenerator);
}

EventManager& GetEventManager()
{
	return (g_application->m_eventManager);
}

GameLogic& GetGameLogic()
{
	return *(g_application->m_gameLogic);
}

ProcessManager& GetProcessManager()
{
	return (g_application->m_processManager);
}

VideoSystem& GetVideoSystem()
{
	return *(g_application->m_videoSystem);
}

std::string GetResourcePath(std::string n_relative)
{
	return g_application->m_resourcePath + n_relative;
}
