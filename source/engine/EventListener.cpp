#include "EventListener.h"
#include "Error.h"
#include <iostream>


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
