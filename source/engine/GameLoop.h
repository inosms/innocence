#ifndef _GAMELOOP_H_
#define _GAMELOOP_H_

#include "Process.h"

/*
This is a threaded game loop!

So this main thread handles the rendering
while the other thread handles the game logic;
therefore there are TWO Update methods:
ViewUpdate and LogicUdpate

The m_lastGameLogicTickTime must be increased by the seperate
game logic thread; this thread just uses the new variable and
updates if the other updated and interpolates between the different
frames
*/


class Thread_Process;
class GameLogic_Thread : public Thread_Process
{
public:
	void VThreadedMethod();
};


class GameLoop
{
	friend class GameLogic_Thread;
protected:

	bool m_running;

	// game logic should only be updated if
	// the specified ticks have passed
	const unsigned int m_gameLogicTicksDelay;

	// when the game logic was updated the last time
	unsigned int m_lastGameLogicTickTime;


	GameLogic_Thread m_gameLogicThread;

public:

	GameLoop();

	// return -1 if failure
	// return 0  if stable exit
	int Run();

	virtual void VViewUpdate() = 0;
	virtual void VLogicUpdate() = 0;
	virtual void VRender(double n_interpolation) = 0;
	virtual void VInput() = 0;

	virtual bool VInit() = 0;
	virtual bool VExit() = 0;

	// sets running to false
	void Kill();

	unsigned int GetLastGameLogicTickTime();
	unsigned int GetGameLogicTicksDelay();
};


#endif
/* _GAMELOOP_H_ */
