#include "inApplication.h"

void test_thread()
{
	EventListener* tmp_listener = new EventListener_DEBUG_OUTPUT();
		for( int i = 0; i < Event_Type_LIST_END; i++ )
		GetEventManager()->AddEventListener( tmp_listener, i );

	Tickmeter tmp_tickmeter("magic_thread");
	int i = 0;
	while( true )
	{
		tmp_tickmeter.Measure();
		GetEventManager()->SendEvent( std::shared_ptr<Event>(new Event_RemoveObject(i++)) );

		if( i % 100 == 0 )
			tmp_listener->ProcessQueuedEvents();
	}
}

GameLogic* inApplication::VCreateGameLogic()
{
	std::thread *t1 = new std::thread(test_thread);

	// TODO THIS IS A DUMMY LOGIC!! (Well it's the basic one Q.Q)
	return new GameLogic();
}
