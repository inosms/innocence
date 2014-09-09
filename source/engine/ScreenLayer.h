#ifndef _SCREENLAYER_H_
#define _SCREENLAYER_H_

#include "Event.h"

enum ScreenLayer_Type
{
	// where the scene graph and the camera resides in
	ScreenLayer_Type_Scene,
	ScreenLayer_Type_CommandLine,

	ScreenLayer_Type_Menu,

	// Same as Event_Type_LIST_END:
	// use this in game to extend the Type list
	ScreenLayer_Type_LIST_END
};

class ScreenLayer
{
private:
	// the screen layer type
	// unsigned int instead of ScreenLayer_Type
	// as usedefined types are allowed
	unsigned int m_type;

public:

	ScreenLayer( unsigned int n_type );

	unsigned int GetType() const;

	virtual void VUpdate() = 0;
	virtual void VRender( double n_interpolation ) = 0;

	// if the game view receives an input event it should forward 
	// the event to the ScreenLayer list which then process the event
	// in desc/asc z-order
	// if the event is consumed, thus not processed by following layers
	// the method should return true
	// otherwise if not consumed return false
	virtual bool VOnEvent(Event_Input& n_event) = 0;

};

#endif