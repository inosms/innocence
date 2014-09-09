#ifndef _EVENT_H_
#define _EVENT_H_

#include "Math.h"

#define FORALLDEFAULTEVENTTYPES(expr) for( int event = 0; event < Event_Type_LIST_END; event++ ) (expr);

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
	Event_Type_MoveSceneNode,
	Event_Type_AddScreenLayer,
	Event_Type_RemoveScreenLayer,

	Event_Type_Input_Key_Down,
	Event_Type_Input_Mousebutton_Down,
	Event_Type_Input_Mousemotion,

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

class Event_MoveSceneNode : public Event
{
public:
	glm::mat4x4 m_mat;
	unsigned int m_id;
	Event_MoveSceneNode(glm::mat4x4 n_mat, unsigned int n_id);
};

class ScreenLayer;
class Event_AddScreenLayer : public Event
{
public:
	ScreenLayer* m_screenLayer;
	Event_AddScreenLayer(ScreenLayer* n_screenLayer);
};

class Event_RemoveScreenLayer : public Event
{
public:
	ScreenLayer* m_screenLayer;
	Event_RemoveScreenLayer(ScreenLayer* n_screenLayer);
};

// this is just a parent class for all kinds of input events
class Event_Input : public Event
{
public: 
	Event_Input( unsigned int n_type );
};

enum Event_Input_Key
{
	A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z
};

class Event_Input_Key_Down : public Event_Input
{
public:
	Event_Input_Key m_key;
	Event_Input_Key_Down(Event_Input_Key n_key);
};

enum Event_Input_Mousebutton
{
	LEFT,
	RIGHT
};

class Event_Input_Mousebutton_Down : public Event_Input
{
public:
	Event_Input_Mousebutton m_button;

	// the click coordiantes
	unsigned int x,y;
	float percent_x, percent_y;
	Event_Input_Mousebutton_Down( Event_Input_Mousebutton n_button, unsigned int n_x, unsigned int n_y, float n_percent_x = 0.f, float n_percent_y = 0.f );
};

class Event_Input_Mousemotion : public Event_Input
{
public:
	int x,y;
	float percent_x,percent_y;
	Event_Input_Mousemotion(int n_x, int n_y, float n_percent_x, float n_percent_y);
};

#endif