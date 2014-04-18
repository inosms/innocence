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

	// to ensure the EventManager is thread safe  
	std::mutex m_mutex;

	// unsigned int is the EventType
	std::map< unsigned int , std::vector<EventListener*> > m_eventToListenerMap;

public:

	// the shared_ptr is neccessary as the threaded
	// event listeners process the event at different times
	// and so deleting the event at the end of send event is not possible
	// this directly send the event to the event queue of all the registered 
	// eventlistener
	void SendEvent( std::shared_ptr<Event> n_event );

	void AddEventListener( EventListener*, unsigned int n_eventType );

	// removes an eventListener for a specific event
	void RemoveEventLister( EventListener*, unsigned int n_eventType );
};

#endif