#include "Event.h"	

Event::Event( unsigned int n_type ) : m_type( n_type ){}

unsigned int Event::GetType()
{
	return m_type;
}


Event_CreateNewObject::Event_CreateNewObject(n_creator* n_creator) : m_creator(n_creator){}

Event_CreateNewObject::~Event_CreateNewObject()
{
	delete m_creator;
}


Event_RemoveObject::Event_RemoveObject(unsigned int n_id) : m_id( n_id )