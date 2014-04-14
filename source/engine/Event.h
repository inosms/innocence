#ifndef _EVENT_H_
#define _EVENT_H_

class Event
{
protected:
	unsigned int m_type;
public:

	Event( unsigned int n_type );

	unsigned int GetType();

	virtual ~Event(){}
};

enum Event_Type
{
	Event_Type_CreateNewObject,
	Event_Type_RemoveObject,

	// yeah the name says it all...
	// when declaring a game specific Event_Type_blablabla
	// the first value should be this
	// (to make sure that the game specific events don't
	// have the same ids at the engine events)
	Event_Type_LIST_END
};

/** Here are some very basic events */

// Used to send a creator which triggers
// the creation of new game logic and game view
// objects.
// When event is destroyed it will also delete the Creator!
class Creator;
class Event_CreateNewObject : public Event
{
public:
	Creator* m_creator;
	Event_CreateNewObject(Creator* n_creator);
	~Event_CreateNewObject();
};

class Event_RemoveObject : public Event
{
public:
	unsigned int m_id;
	Event_RemoveObject(unsigned int n_id);
};

#endif