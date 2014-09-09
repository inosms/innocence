#include "inHumanViewListener.h"

inHumanViewListener::inHumanViewListener(inHumanView* n_view) : 
	HumanViewListener(n_view)
{
}

void inHumanViewListener::VProcessEvent( Event& n_event )
{
	std::cout << "event!" << n_event.GetType() << std::endl;

	// default handling
	HumanViewListener::VProcessEvent(n_event);

}
