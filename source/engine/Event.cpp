#include "Event.h"	
Event::Event( unsigned int n_type ) : m_type( n_type ){}

unsigned int Event::GetType()
{
	return m_type;
}