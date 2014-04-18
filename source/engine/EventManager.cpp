#include "EventManager.h"

void EventManager::SendEvent( std::shared_ptr<Event> n_event )
{
	std::lock_guard<std::mutex> tmp_lock(m_mutex);
	// get EventListeners for this event
	auto tmp_find = m_eventToListenerMap.find(n_event->GetType());
	if( tmp_find != m_eventToListenerMap.end() )
	{
		// iterate eventlisteners
		for( EventListener* i_event_listener : tmp_find->second )
		{
			// process current Event with every event listener
			i_event_listener->QueueEvent(n_event);
		}
	}
}

void EventManager::AddEventListener( EventListener* n_eventListener, unsigned int n_event )
{
	// http://en.cppreference.com/w/cpp/thread/lock_guard
	// for thread safe event listener adding 
	std::lock_guard<std::mutex> tmp_lock(m_mutex);
	m_eventToListenerMap[n_event].push_back(n_eventListener);	
}

void EventManager::RemoveEventLister( EventListener*, unsigned int n_event )
{
	std::lock_guard<std::mutex> tmp_lock(m_mutex);

	// TODO 
}