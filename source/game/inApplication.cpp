#include "inApplication.h"

GameLogic* inApplication::VCreateGameLogic()
{
	EventListener* tmp_listener = new EventListener_DEBUG_OUTPUT();
	for( int i = 0; i < Event_Type_LIST_END; i++ )
		GetEventManager()->AddEventListener( tmp_listener, i );

	GetEventManager()->QueueEvent( new Event_RemoveObject(42) );

	// TODO THIS IS A DUMMY LOGIC!! (Well it's the basic one Q.Q)
	return new GameLogic();
}
