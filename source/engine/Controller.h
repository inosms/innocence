#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "Event.h"

class Controller
{
public:
    virtual void VOnKeyDown(Event_Input_Key n_key){}
    virtual void VOnKeyUp(Event_Input_Key n_key){}

    virtual void VOnMouseDown(Event_Input_Mousebutton n_button){}
    virtual void VOnMouseUp(Event_Input_Mousebutton n_button){}

    virtual void VOnMouseMotion(Event_Input_Mousemotion n_event){}

    // this just forwards the events to the correct
    // function here
    void OnEvent(Event& n_event);
};

#endif
