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
	// this is increased by TicksDelay each time
	unsigned int m_lastGameLogicTickTime;

	// the real time from SDL the game logic was ticked the last time
	// this is really the REALY time, which is very useful for interpolating
	// correctly when the gamelogic gets very slow
	// (as then LastTickTime is waay back and interpolation gets veery high,
	// which looks like a ol drunk engine then)
	unsigned int m_realLastGameLogicTickTime;

	GameLogic_Thread m_gameLogicThread;

	// whether interpolation value is calculated or just 0 ( no interpolation) is passed
	bool m_calculateInterpolation = true;
	// the maximum interpolation value, this prevents the interpolation
	// from exploding
	const float m_maxInterpolationValue = 1.5f;
public:

	GameLoop();

	// return -1 if failure
	// return 0  if stable exit
	int Run();

	virtual void VViewUpdate() = 0;
	virtual void VLogicUpdate() = 0;
	virtual void VRender(double n_interpolation) = 0;
	virtual void VInput() = 0;

	virtual void VInit() = 0;
	virtual void VExit() = 0;

	// sets running to false
	void Kill();

	void SetInterpolationEnabled(bool n_enabled);
	unsigned int GetLastGameLogicTickTime();
	unsigned int GetGameLogicTicksDelay();
};


#endif
/* _GAMELOOP_H_ */
