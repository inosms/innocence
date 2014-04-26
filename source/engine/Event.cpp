#include "Event.h"	
#include "Creator.h"

Event::Event( unsigned int n_type ) : 
	m_type( n_type )
	{}

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

Event_Input::Event_Input( unsigned int n_type ) :
	Event(n_type)
{}

Event_Input_Key_Down::Event_Input_Key_Down(Event_Input_Key n_key) :
	Event_Input(Event_Type_Input_Key_Down),
	m_key(n_key)
{

}