#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "GameLogic.h"
#include "GameLoop.h"
#include "GameView.h"
#include "Math.h"
#include "VideoSystem.h"
#include "UniqueNumberGenerator.h"
#include "EventManager.h"
#include "Tickmeter.h"
#include "ProcessManager.h"

class Application : public GameLoop
{
public:
	/** IMPORTANT GAME SUBSYSTEMS */
	std::unique_ptr<VideoSystem> m_videoSystem;
	std::unique_ptr<GameLogic> m_gameLogic;

	std::vector<std::unique_ptr<GameView> > m_gameViews;

	UniqueNumberGenerator m_uniqueNumberGenerator;
	EventManager m_eventManager;
	ProcessManager m_processManager;
	/**/

	// the path to the resource folder
	std::string m_resourcePath = "../resources/";

	Application();

	virtual void VViewUpdate();
	virtual void VLogicUpdate();
	virtual void VRender(double n_interpolation);
	virtual void VInput();

	virtual void VInit();
	virtual void VExit();

	void AddGameView(GameView* n_gameView);

	// remove game view by game view id
	void RemoveGameView( unsigned int n_id );

	// MUST be implemented by actual game to offer
	// the game's game logic (and not some dummy logic...)
	virtual GameLogic* VCreateGameLogic() = 0;
};

extern Application* g_application;
extern UniqueNumberGenerator& GetUniqueNumberGenerator();
extern EventManager& GetEventManager();
extern GameLogic& GetGameLogic();
extern ProcessManager& GetProcessManager();
extern VideoSystem& GetVideoSystem();
// input relative path to resource path output path plus resourcepath
extern std::string GetResourcePath(std::string n_relative);

#endif
/* _APPLICATION_H_ */
