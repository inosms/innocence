#include "Event.h"	
#include "Creator.h"

Event::Event( unsigned int n_type ) : m_type( n_type ){}

unsigned int Event::GetType()
{
	return m_type;
}


Event_CreateNewObject::Event_CreateNewObject(Creator* n_creator) : 
	Event(Event_Type_CreateNewObject),
	m_creator(n_creator)
	{}

Event_CreateNewObject::~Event_CreateNewObject()
{
	delete m_creator;
}


Event_RemoveObject::Event_RemoveObject(unsigned int n_id) : 
	Event(Event_Type_RemoveObject),
	m_id( n_id ) 
	{}