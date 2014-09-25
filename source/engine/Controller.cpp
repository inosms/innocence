#include "Controller.h"

void Controller::OnEvent(Event& n_event)
{
    if( n_event.GetType() == Event_Type_Input_Key_Down)
        VOnKeyDown(dynamic_cast<Event_Input_Key_Down&>(n_event).m_key);
    // TODO: on key up
    else if( n_event.GetType() == Event_Type_Input_Mousebutton_Down )
        VOnMouseDown(dynamic_cast<Event_Input_Mousebutton_Down&>(n_event).m_button);
    // TODO: on mouse button up
    else if( n_event.GetType() == Event_Type_Input_Mousemotion )
        VOnMouseMotion(dynamic_cast<Event_Input_Mousemotion&>(n_event));
}
