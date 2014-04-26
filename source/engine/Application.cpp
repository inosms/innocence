#include "Application.h"
#include "Error.h"
#include "VideoSystem_SDL_OpenGL.h"

Application* g_application = nullptr ;

Application::Application()
{
	m_videoSystem = new VideoSystem_SDL_OpenGL();

	g_application = this;
}

void Application::VUpdate()
{
	PEDANTIC_DEBUG_MESSAGE("VUpdate() called");

	for( GameView* i_gameView : m_gameViews )
	{
		// only update the AI GameViews as the Human GameViews
		// (should) have their own thread
		if( i_gameView->GetGameViewType() == GameView_Type_AI )
			i_gameView->VUpdate();
	}
	
	m_processManager.Update();
	m_gameLogic->VUpdate();
}

void Application::VInput()
{
	m_videoSystem->VTranslateInput();
}

bool Application::VInit()
{
	m_gameLogic = VCreateGameLogic();	
	if( !m_gameLogic ) return false;
	if( !m_videoSystem->VInit() ) return false;

	// TODO 
	
	return true;
}

bool Application::VExit()
{
	// try to stop all processes
	m_processManager.KillAllProcesses();
	DEBUG_MESSAGE("Waiting for all processes to shut down");
		while( m_processManager.AllProcessesDead() == false )
		{
			m_processManager.Update();
		}
	DEBUG_MESSAGE("All processes shut down");

	if( !m_videoSystem->VExit() ) return false;


	// TODO 

	DEBUG_MESSAGE("clean exit");

	return true;
}

void Application::AddGameView(GameView* n_gameView)
{
	assert(n_gameView!=nullptr);
	n_gameView->VInit();
	m_gameViews.push_back(n_gameView);
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

UniqueNumberGenerator* GetUniqueNumberGenerator()
{
	return &(g_application->m_uniqueNumberGenerator);
}

EventManager* GetEventManager()
{
	return &(g_application->m_eventManager);
}

GameLogic* GetGameLogic()
{
	return (g_application->m_gameLogic);
}

ProcessManager* GetProcessManager()
{
	return &(g_application->m_processManager);
}

VideoSystem* GetVideoSystem()
{
	return g_application->m_videoSystem;
}
