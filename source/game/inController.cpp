#include "inController.h"
#include "inEvent.h"

void inController::VOnKeyDown(Event_Input_Key n_key)
{
    if( n_key == Event_Input_Key::RIGHT )
        SEND_EVENT(Event_RequestPlayerMove,glm::vec2(1.0f,0.f));
    else if( n_key == Event_Input_Key::LEFT )
        SEND_EVENT(Event_RequestPlayerMove,glm::vec2(-1.0f,0.f));
    else if( n_key == Event_Input_Key::SPACE )
        SEND_EVENT(Event_RequestPlayerJump);
}
