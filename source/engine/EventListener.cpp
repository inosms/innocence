#include "EventListener.h"
#include "Error.h"

bool EventListener_DEBUG_OUTPUT::VProcessEvent( Event* n_event )
{
	DEBUG_MESSAGE(n_event->GetType());

	// event was not consumed
	return false;
}
