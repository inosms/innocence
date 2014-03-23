#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <iostream>
#include "GameLogic.h"
#include "GameLoop.h"
#include "VideoSystem.h"
#include "UniqueNumberGenerator.h"
#include "EventManager.h"


class Application : public GameLoop
{
public:

	/** IMPORTANT GAME SUBSYSTEMS */
	VideoSystem* m_videoSystem;
	GameLogic* m_gameLogic;

	UniqueNumberGenerator m_uniqueNumberGenerator;
	EventManager m_eventManager;
	/**/

	Application();

	virtual void VUpdate();
	virtual void VRender(double n_interpolation);
	virtual void VInput();

	virtual bool VInit();
	virtual bool VExit();

	// MUST be implemented by actual game to offer 
	// the game's game logic (and not some dummy logic...)
	virtual GameLogic* CreateGameLogic() = 0;
};

extern Application* g_application;
extern UniqueNumberGenerator* GetUniqueNumberGenerator();
extern EventManager* GetEventManager();
extern GameLogic* GetGameLogic();


#endif 
/* _APPLICATION_H_ */