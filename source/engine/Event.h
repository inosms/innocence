#ifndef _EVENT_H_
#define _EVENT_H_

class Event
{
protected:
	unsigned int m_type;
public:
	
	Event( unsigned int n_type );

	unsigned int GetType();
};

#endif