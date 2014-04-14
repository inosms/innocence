#include "EventManager.h"

EventManager::EventManager() : m_currentQueue(0)
{}

void EventManager::QueueEvent( Event* n_event )
{
	// http://en.cppreference.com/w/cpp/thread/lock_guard
	// for thread safe event queueing 
	std::lock_guard<std::mutex> tmp_lock(m_mutex);

	m_eventQueue[m_currentQueue].push_back(n_event);
}

void EventManager::ProcessEvents()
{
	unsigned int tmp_indexForProcessedQueue = m_currentQueue;
	// for thread safe event queue switching	
	{
		std::lock_guard<std::mutex> tmp_lock(m_mutex);

		// process the "old" current queue and switch the queue
		// that new elements are no inserted in the currently processed queue
		m_currentQueue = (m_currentQueue+1)%2;
	}

	// iterate the events
	for( Event* i_event : m_eventQueue[tmp_indexForProcessedQueue] )
	{
		// get EventListeners for this event
		auto tmp_find = m_eventToListenerMap.find(i_event->GetType());
		if( tmp_find != m_eventToListenerMap.end() )
		{
			// iterate eventlisteners
			for( EventListener* i_event_listener : tmp_find->second )
			{
				// process current Event with every event listener
				// until one returns true (means the evenlistener has fully 
				// consumed the event)
				if( i_event_listener->VProcessEvent(*i_event) == true )
					break;
			}
		}

		// after event was processed is must be deleted
		delete i_event;
	}

	// finally the whole eventqueue, which is now full with empty pointers
	// must be cleared
	m_eventQueue[tmp_indexForProcessedQueue].clear();
}

void EventManager::AddEventListener( EventListener* n_eventListener, unsigned int n_event )
{
	m_eventToListenerMap[n_event].push_back(n_eventListener);	
}

void EventManager::RemoveEventLister( EventListener*, unsigned int n_event )
{
	// TODO 
}