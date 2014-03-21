#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <iostream>
#include "GameLoop.h"
#include "VideoSystem.h"
#include "UniqueNumberGenerator.h"
#include "EventManager.h"


class Application : public GameLoop
{
public:

	/** IMPORTANT GAME SUBSYSTEMS */
	VideoSystem* m_videoSystem;
	UniqueNumberGenerator m_uniqueNumberGenerator;
	EventManager m_eventManager;
	/**/

	Application();

	virtual void VUpdate();
	virtual void VRender(double n_interpolation);
	virtual void VInput();

	virtual bool VInit();
	virtual bool VExit();
};

extern Application* g_application;
extern UniqueNumberGenerator* GetUniqueNumberGenerator();
extern EventManager* GetEventManager();


#endif 
/* _APPLICATION_H_ */