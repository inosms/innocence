#ifndef _MENU_H_
#define _MENU_H_

#include "ScreenLayer.h"

class Menu : public ScreenLayer
{
public:
	Menu();

	// removes this and goes to next
	void GoToNextScreenLayer(ScreenLayer* n_next);

	// sends event to remove this
	void RemoveThis();

	// returns true if menu is active
	// an inactive menu is caused by traversing to the next 
	// screen layer - between requesting and deleting 
	// the timespan no events should be fired (double traverse, etc)
	// so checking this would be nice...
	bool m_active = true;
	bool IsActive();

};

#endif