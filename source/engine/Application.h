#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <iostream>
#include <vector>

#include "GameLogic.h"
#include "GameLoop.h"
#include "GameView.h"
#include "VideoSystem.h"
#include "UniqueNumberGenerator.h"
#include "EventManager.h"
#include "Tickmeter.h"
#include "ProcessManager.h"

class Application : public GameLoop
{
public:

	/** IMPORTANT GAME SUBSYSTEMS */
	VideoSystem* m_videoSystem;
	GameLogic* m_gameLogic;

	std::vector<GameView*> m_gameViews;

	UniqueNumberGenerator m_uniqueNumberGenerator;
	EventManager m_eventManager;
	ProcessManager m_processManager;
	/**/

	Application();

	virtual void VUpdate();
	virtual void VRender(double n_interpolation);
	virtual void VInput();

	virtual bool VInit();
	virtual bool VExit();

	// MUST be implemented by actual game to offer 
	// the game's game logic (and not some dummy logic...)
	virtual GameLogic* VCreateGameLogic() = 0;
};

extern Application* g_application;
extern UniqueNumberGenerator* GetUniqueNumberGenerator();
extern EventManager* GetEventManager();
extern GameLogic* GetGameLogic();
extern ProcessManager* GetProcessManager();


#endif 
/* _APPLICATION_H_ */