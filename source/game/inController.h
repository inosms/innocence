#ifndef _INCONTROLLER_H_
#define _INCONTROLLER_H_

#include "../engine/engine.h"

class inController : public Controller
{
public:
    virtual void VOnKeyDown(Event_Input_Key n_key);
    virtual void VOnKeyUp(Event_Input_Key n_key);
};

#endif
