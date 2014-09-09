#include "inHumanViewListener.h"

inHumanViewListener::inHumanViewListener(inHumanView* n_view) : 
	m_view(n_view)
{
}

void inHumanViewListener::VProcessEvent( Event& n_event )
{
	if( n_event.GetType() == Event_Type_CreateNewObject )
	{
		Event_CreateNewObject& tmp_event = dynamic_cast<Event_CreateNewObject&>(n_event);
		tmp_event.m_creator->CreateSceneNode(*m_view);
	}
}
