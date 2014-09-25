#ifndef _INEVENT_H_
#define _INEVENT_H_

#include "../engine/engine.h"

enum inEvent_Type
{
    // starting with the end of the default events of the engine
    inEvent_Type_RequestPlayerMove = Event_Type_LIST_END,
    inEvent_Type_RequestPlayerJump
};

class Event_RequestPlayerMove : public Event
{
public:
    glm::vec2 m_direction;
    Event_RequestPlayerMove(glm::vec2 n_direction);
};

class Event_RequestPlayerJump : public Event
{
public:
    Event_RequestPlayerJump();
};

#endif
