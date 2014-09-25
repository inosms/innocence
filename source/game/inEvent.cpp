#include "inEvent.h"

Event_RequestPlayerMove::Event_RequestPlayerMove(glm::vec2 n_direction) :
    Event(inEvent_Type_RequestPlayerMove),
    m_direction(n_direction)
{}

Event_RequestPlayerJump::Event_RequestPlayerJump() :
    Event(inEvent_Type_RequestPlayerJump)
{}
