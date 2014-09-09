#ifndef _INHUMANVIEWLISTENER_H_
#define _INHUMANVIEWLISTENER_H_

#include "../engine/engine.h"
#include "inHumanView.h"
class inHumanViewListener : public EventListener
{
	inHumanView* m_view;
public:
	inHumanViewListener(inHumanView* n_view);
	virtual void VProcessEvent( Event& n_event );
};
#endif