#ifndef _INAPPLICATION_H_
#define _INAPPLICATION_H_

#include "../engine/Application.h"

class inApplication : public Application
{
public:
	inApplication();
	GameLogic* VCreateGameLogic();
};

#endif