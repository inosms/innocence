#include "GameView.h"
#include "Application.h"
#include "VideoSystem_SDL_OpenGL.h"
#include "Error.h"
#include "ScreenLayer_Scene.h"
#include <algorithm>

GameView::GameView( GameView_Type n_type ) :
	m_gameViewType( n_type ),
	m_id(GenerateGameViewID())
	{}

unsigned int GameView::GenerateGameViewID()
{
	static unsigned int tmp_id = 1;
	return tmp_id++;
}

GameView_Type GameView::GetGameViewType()
{
	return m_gameViewType;
}

unsigned int GameView::GetID()
{
	return m_id;
}


GameView_Human::GameView_Human(EventListener* n_listener) :
	GameView(GameView_Type_Human),
	m_listener( n_listener )
	{
	}

void GameView_Human::VRender( double n_interpolation )
{
	GetVideoSystem()->VClearScreen();

	for( auto i_layer : m_screenLayers )
		i_layer->VRender(n_interpolation);

	GetVideoSystem()->VUpdateScreen();	
}

void GameView_Human::VUpdate()
{
	for( auto i_layer : m_screenLayers )
		i_layer->VUpdate();
	m_listener->ProcessQueuedEvents();
}

void GameView_Human::VInit()
{
	m_scene = new ScreenLayer_Scene();
	AddScreenLayer( m_scene );
}

void GameView_Human::AddScreenLayer( ScreenLayer* n_layer )
{
	assert( n_layer != nullptr );
	m_screenLayers.push_back( n_layer );
	std::sort(m_screenLayers.begin(),m_screenLayers.end(),[](const ScreenLayer* a, const ScreenLayer* b){return a->GetType() < b->GetType();});
}

ScreenLayer_Scene* GameView_Human::GetScene()
{
	return m_scene;
}


HumanView_Process::HumanView_Process(GameView_Human* n_view):
	m_view(n_view)
	{}


void HumanView_Process::VInitialize()
{
	m_lastTickTime = g_application->GetLastGameLogicTickTime();
	m_tickDelayTime = g_application->GetGameLogicTicksDelay();

	Thread_Process::VInitialize();
}

void HumanView_Process::VThreadedMethod()
{
	while( m_running )
	{
		while( SDL_GetTicks() >= m_lastTickTime + m_tickDelayTime )
		{
			m_view->VUpdate();
			m_lastTickTime += m_tickDelayTime;
		}

		double tmp_interpolation = (SDL_GetTicks() - m_lastTickTime)/double(m_tickDelayTime);
		m_view->VRender(tmp_interpolation);
	}
}
