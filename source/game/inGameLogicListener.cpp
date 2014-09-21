#include "inGameLogicListener.h"


inGameLogicListener::inGameLogicListener(inGameLogic* n_logic) :
	GameLogicListener(n_logic)
{

}

void inGameLogicListener::VProcessEvent( Event& n_event )
{
	// default handling
	GameLogicListener::VProcessEvent(n_event);

	inGameLogic* tmp_logic = dynamic_cast<inGameLogic*>(m_logic);
	if( n_event.GetType() == Event_Type_LoadLevel )
	{
		Event_LoadLevel& tmp_event = dynamic_cast<Event_LoadLevel&>(n_event);
		tmp_logic->Load(tmp_event.m_levelXMLName);
	}
}