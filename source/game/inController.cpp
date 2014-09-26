#include "inController.h"
#include "inEvent.h"
#include "inGameObject.h"

void inController::VOnKeyDown(Event_Input_Key n_key)
{
    if( n_key == Event_Input_Key::RIGHT )
        SEND_EVENT(Event_RequestPlayerMove,glm::vec2(1.0f,0.f));
    else if( n_key == Event_Input_Key::LEFT )
        SEND_EVENT(Event_RequestPlayerMove,glm::vec2(-1.0f,0.f));
    else if( n_key == Event_Input_Key::SPACE )
        SEND_EVENT(Event_RequestPlayerJump);
    else if( n_key == Event_Input_Key::N )
    {
        for(int i = 0; i < 5; i++ ) SEND_EVENT(Event_CreateNewObject, new Creator_DynamicTest() );
        static int count = 0;
        count += 5;
        INFO("created " << count << " dynamic objects" );
    }
    else if( n_key == Event_Input_Key::I )
    {
        static bool tmp_state = true;
        g_application->SetInterpolationEnabled(tmp_state);
        tmp_state = !tmp_state;
    }
}

void inController::VOnKeyUp(Event_Input_Key n_key)
{
    if( n_key == Event_Input_Key::RIGHT || n_key == Event_Input_Key::LEFT )
        SEND_EVENT(Event_RequestPlayerStop);
}
