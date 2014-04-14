#ifndef _EVENTMANAGER_H_
#define _EVENTMANAGER_H_

#include "Event.h"
#include "EventListener.h"
#include <vector>
#include <map>
#include <mutex>
#include <thread>

class EventManager
{
private:

	// to ensure EventManager QueueEvent(Event*) is thread safe
	std::mutex m_mutex;

	// two event queues are used in order to
	// make sure that while processing one queue
	// the new events are stored in the other queue
	// (no endless queue processing)
	std::vector<Event*> m_eventQueue[2];

	// stores wich queue is currently filled with new events
	int m_currentQueue;

	// unsigned int is the EventType
	std::map< unsigned int , std::vector<EventListener*> > m_eventToListenerMap;

public:

	EventManager();

	void QueueEvent( Event* n_event );

	// processes all queued events
	void ProcessEvents();

	void AddEventListener( EventListener*, unsigned int n_eventType );

	// removes an eventListener for a specific event
	void RemoveEventLister( EventListener*, unsigned int n_eventType );
};

#endif