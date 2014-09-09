#include "Event.h"	
#include "Creator.h"
#include "ScreenLayer.h"
#include <iostream>

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

Event_MoveSceneNode::Event_MoveSceneNode(glm::mat4x4 n_mat, unsigned int n_id) :
	Event(Event_Type_MoveSceneNode),
	m_mat(n_mat),
	m_id(n_id)
{}

Event_AddScreenLayer::Event_AddScreenLayer(ScreenLayer* n_screenLayer)
	: Event(Event_Type_AddScreenLayer),
	m_screenLayer(n_screenLayer)
{}

Event_RemoveScreenLayer::Event_RemoveScreenLayer(ScreenLayer* n_screenLayer) :
	Event(Event_Type_RemoveScreenLayer),
	m_screenLayer(n_screenLayer)
{}

Event_Input::Event_Input( unsigned int n_type ) :
	Event(n_type)
{}

Event_Input_Key_Down::Event_Input_Key_Down(Event_Input_Key n_key) :
	Event_Input(Event_Type_Input_Key_Down),
	m_key(n_key)
{

}

Event_Input_Mousebutton_Down::Event_Input_Mousebutton_Down( Event_Input_Mousebutton n_button , unsigned int n_x, unsigned int n_y , float n_percent_x, float n_percent_y ): 
	Event_Input(Event_Type_Input_Mousebutton_Down),
	m_button(n_button),
	x(n_x),
	y(n_y),
	percent_x(n_percent_x),
	percent_y(n_percent_y)
{}

Event_Input_Mousemotion::Event_Input_Mousemotion(int n_x, int n_y, float n_percent_x, float n_percent_y) :
	Event_Input(Event_Type_Input_Mousemotion),
	x(n_x),
	y(n_y),
	percent_x(n_percent_x),
	percent_y(n_percent_y)
	{}