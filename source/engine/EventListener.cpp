#include "EventListener.h"
#include "Error.h"
#include <iostream>

bool EventListener_DEBUG_OUTPUT::VProcessEvent( Event& n_event )
{
	DEBUG_MESSAGE(n_event.GetType());

	if( n_event.GetType() == Event_Type_RemoveObject )
	{
		std::cout << "REMOVAL OF " << dynamic_cast<Event_RemoveObject&>(n_event).m_id << std::endl;
	}

	// event was not consumed
	return false;
}
