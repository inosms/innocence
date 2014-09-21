#ifndef _INGAMELOGICLISTENER_H_
#define _INGAMELOGICLISTENER_H_

#include "../engine/engine.h"
#include "inGameLogic.h"

class inGameLogicListener : public GameLogicListener
{
public:
	inGameLogicListener(inGameLogic* n_logic);
	virtual void VProcessEvent( Event& n_event );
};

#endif