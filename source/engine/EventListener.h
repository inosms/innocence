#ifndef _EVENTLISTENER_H_
#define _EVENTLISTENER_H_

#include "Event.h"

class EventListener
{
public:

	// is called when event arrives;
	// return false if event is not consumed
	// return true if event is consumed and no further
	// EventListener should receive this Event
	virtual bool VProcessEvent( Event* n_event ) = 0;
};

class EventListener_DEBUG_OUTPUT : public EventListener
{
public:

	bool VProcessEvent( Event* n_event );
};

#endif