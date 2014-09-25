#ifndef _INGAMELOGIC_H_
#define _INGAMELOGIC_H_

#include "../engine/engine.h"

class inGameLogic : public GameLogic
{
public:
	inGameLogic();

	//virtual void VUpdate(){GameLogic::VUpdate();}

	void Load(std::string n_levelXML);

	// returns the player
	GameObject* GetPlayer();

	// whether there is an active level running
	bool m_levelStarted = false;
};

#endif
