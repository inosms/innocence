#ifndef _INHUMANVIEWLISTENER_H_
#define _INHUMANVIEWLISTENER_H_

#include "../engine/engine.h"
#include "inHumanView.h"
class inHumanViewListener : public HumanViewListener
{
public:
	inHumanViewListener(inHumanView* n_view);
	virtual void VProcessEvent( Event& n_event );
};
#endif