#ifndef _GAMELOOP_H_
#define _GAMELOOP_H_

class GameLoop
{
protected:

	bool m_running;

	// game logic should only be updated if
	// the specified ticks have passed
	const unsigned int m_gameLogicTicksDelay;

	// when the game logic was updated the last time
	unsigned int m_lastGameLogicTickTime;

	// the maximum number of frames that can be skipped 
	// if the computer can't keep up and the logic is 
	// updated multiple times before rendering occurs
	unsigned short m_maxFrameSkips;

public:

	GameLoop();

	// return -1 if failure
	// return 0  if stable exit
	int Run();

	virtual void VUpdate() = 0;
	//virtual void VRender(double n_interpolation) = 0;
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