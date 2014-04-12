#include "EventManager.h"

EventManager::EventManager() : m_currentQueue(0)
{}

void EventManager::QueueEvent( Event* n_event )
{
	m_eventQueue[m_currentQueue].push_back(n_event);
}

void EventManager::ProcessEvents()
{
	// process the "old" current queue and switch the queue
	// that new elements are no inserted in the currently processed queue
	unsigned int tmp_indexForProcessedQueue = m_currentQueue;
	m_currentQueue = (m_currentQueue+1)%2;

	// iterate the events
	for( unsigned int i = 0; i < m_eventQueue[tmp_indexForProcessedQueue].size(); i++ )
	{
		// get EventListeners for this event
		std::map<unsigned int, std::vector<EventListener*> >::iterator tmp_find = m_eventToListenerMap.find(m_eventQueue[tmp_indexForProcessedQueue][i]->GetType());
		if( tmp_find != m_eventToListenerMap.end() )
		{
			// iterate eventlisteners
			for( unsigned int j = 0; j < tmp_find->second.size(); j++ )
			{
				// process current Event with every event listener
				// until one returns true (means the evenlistener has fully 
				// consumed the event)
				if( tmp_find->second[j]->VProcessEvent(*(m_eventQueue[tmp_indexForProcessedQueue][i])) == true )
					break;
			}
		}

		// after event was processed is must be deleted
		delete m_eventQueue[tmp_indexForProcessedQueue][i];
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