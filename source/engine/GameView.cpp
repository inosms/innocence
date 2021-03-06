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
		// add all default event types
		FORALLDEFAULTEVENTTYPES(GetEventManager().AddEventListener(m_listener,event));
	}

void GameView_Human::VRender( double n_interpolation )
{
	GetVideoSystem().VClearScreen();

	for( auto i_layer : m_screenLayers )
		i_layer->VRender(n_interpolation);

	GetVideoSystem().VUpdateScreen();
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

ScreenLayer* GameView_Human::GetScreenLayer( ScreenLayer_Type n_type )
{
	for( auto i_layer : m_screenLayers )
		if( i_layer->GetType() == n_type )
			return i_layer;
	return nullptr;
}

void GameView_Human::RemoveScreenLayer(ScreenLayer* n_layer )
{
	auto tmp_find = std::find(m_screenLayers.begin(), m_screenLayers.end(), n_layer);
	if( tmp_find != m_screenLayers.end() )
	m_screenLayers.erase(tmp_find);
}

void GameView_Human::ForwardInputEvent( Event& n_event )
{
	for( auto i_layer : m_screenLayers )
		if(i_layer->VOnEvent(n_event)) return;

	if(m_controller) m_controller->OnEvent(n_event);
}

ScreenLayer_Scene* GameView_Human::GetScene()
{
	return m_scene;
}
