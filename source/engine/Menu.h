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

};

#endif