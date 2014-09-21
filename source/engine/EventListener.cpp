#include "EventListener.h"
#include "Error.h"
#include <iostream>
#include "engine.h"

void EventListener::QueueEvent(std::shared_ptr<Event> n_event)
{
	std::lock_guard<std::mutex> tmp_lock(m_mutex);
	m_queue[m_currentQueue].push_back(n_event);
}

void EventListener::ProcessQueuedEvents()
{
	unsigned int tmp_indexForProcessedQueue = m_currentQueue;
	// for thread safe event queue switching	
	{
		std::lock_guard<std::mutex> tmp_lock(m_mutex);

		// process the "old" current queue and switch the queue
		// that new elements are no inserted in the currently processed queue
		m_currentQueue = (m_currentQueue+1)%2;
	}

	// Process all elements in queue
	for( auto i_event : m_queue[tmp_indexForProcessedQueue] )
		VProcessEvent(*i_event);

	// as all events are processed empty the queue
	m_queue[tmp_indexForProcessedQueue].clear();
}

void EventListener_DEBUG_OUTPUT::VProcessEvent( Event& n_event )
{
	DEBUG_MESSAGE(n_event.GetType());

	if( n_event.GetType() == Event_Type_RemoveObject )
	{
		//std::cout << "REMOVAL OF " << dynamic_cast<Event_RemoveObject&>(n_event).m_id << std::endl;
	}
}

HumanViewListener::HumanViewListener(GameView_Human* n_view) : m_view(n_view)
{

}

void HumanViewListener::VProcessEvent( Event& n_event )
{
	if( n_event.GetType() == Event_Type_CreateNewObject )
	{
		Event_CreateNewObject& tmp_event = dynamic_cast<Event_CreateNewObject&>(n_event);
		tmp_event.m_creator->CreateSceneNode(*m_view);
	}
	else if( n_event.GetType() == Event_Type_AddScreenLayer )
	{
		Event_AddScreenLayer& tmp_event = dynamic_cast<Event_AddScreenLayer&>(n_event);
		m_view->AddScreenLayer(tmp_event.m_screenLayer);
	}
	else if( n_event.GetType() == Event_Type_RemoveScreenLayer )
	{
		Event_RemoveScreenLayer& tmp_event = dynamic_cast<Event_RemoveScreenLayer&>(n_event);
		m_view->RemoveScreenLayer(tmp_event.m_screenLayer);
	}
	else if( n_event.GetType() > EVENT_TYPE_FORWARD_TO_SCREEN_LAYER_START  && n_event.GetType() < EVENT_TYPE_FORWARD_TO_SCREEN_LAYER_END )
	{
		m_view->ForwardInputEvent(n_event);
	} 
}

GameLogicListener::GameLogicListener(GameLogic* n_logic) : m_logic(n_logic)
{}

void GameLogicListener::VProcessEvent( Event& n_event )
{
	if( n_event.GetType() == Event_Type_SetGameState )
	{
		Event_SetGameState& tmp_event = dynamic_cast<Event_SetGameState&>(n_event);
		m_logic->SetState(tmp_event.m_state);
	}
	else if( n_event.GetType() == Event_Type_CreateNewObject )
	{
		Event_CreateNewObject& tmp_event = dynamic_cast<Event_CreateNewObject&>(n_event);
		tmp_event.m_creator->CreateGameObject(*m_logic);
	}
}
