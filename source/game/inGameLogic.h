#ifndef _INGAMELOGIC_H_
#define _INGAMELOGIC_H_

#include "../engine/engine.h"

class inGameLogic : public GameLogic
{
public:
	inGameLogic();

	//virtual void VUpdate(){GameLogic::VUpdate();}

	void Load(std::string n_levelXML);
};

#endif