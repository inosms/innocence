#include "inGameLogicListener.h"
#include "inEvent.h"
#include "inGameObject.h"

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
	else if( n_event.GetType() == inEvent_Type_RequestPlayerMove )
	{
		Event_RequestPlayerMove& tmp_event = dynamic_cast<Event_RequestPlayerMove&>(n_event);
		GameObject_PlayerTest* tmp_player = dynamic_cast<GameObject_PlayerTest*>(tmp_logic->GetPlayer());
		if( tmp_player )
		{
			tmp_player->OnRequestPlayerMove(tmp_event.m_direction);
		}
	}
	else if( n_event.GetType() == inEvent_Type_RequestPlayerJump )
	{
		GameObject_PlayerTest* tmp_player = dynamic_cast<GameObject_PlayerTest*>(tmp_logic->GetPlayer());
		if( tmp_player )
		{
			tmp_player->OnRequestPlayerJump();
		}
	}
	else if( n_event.GetType() == inEvent_Type_RequestPlayerStop )
	{
		GameObject_PlayerTest* tmp_player = dynamic_cast<GameObject_PlayerTest*>(tmp_logic->GetPlayer());
		if( tmp_player )
		{
			tmp_player->OnRequestPlayerStop();
		}
	}
}
