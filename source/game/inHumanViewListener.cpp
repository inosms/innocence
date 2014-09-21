#include "inHumanViewListener.h"

inHumanViewListener::inHumanViewListener(inHumanView* n_view) : 
	HumanViewListener(n_view)
{
}

void inHumanViewListener::VProcessEvent( Event& n_event )
{
	// default handling
	HumanViewListener::VProcessEvent(n_event);

	inHumanView* tmp_view = dynamic_cast<inHumanView*>(m_view);

}
