#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "UniqueNumberGenerator.h"
#include <vector>

class Process
{
private:

	// if a process is dead it is removed from the process
	// manager an the m_next Process (if any) can get executed 
	bool m_dead = false;

	// this allows for process chaining
	Process* m_next = nullptr;

	// this id is assigned when the process is attached
	// to the process manager; the process manager
	// then genereates a unique ID for this process
	// which will (hopefully) be unqique troughout the 
	// whole runtime of the game
	// if you let this thing run for several days in a row
	// ... well ... then ... 
	unsigned int m_id = UniqueNumberGenerator::NO_ID;

public:

	unsigned int GetID();
	void SetID(unsigned int n_id);
	bool IsDead();

	Process* GetNext();
	void SetNext( Process* n_process );

	virtual void VUpdate() = 0;

	// send the process the signal to stop
	// does not have to stop IMMEDIATELY if threaded
	// process; but pls try to do diz as fast as possibru Q.Q
	virtual void VOnRequestKill() = 0;

	// go through this childs and if this or the child with given id is found
	// kill it! WITH FIREEE
	void FindAndKill(unsigned int n_id);

	// kills this and all child processes
	void KillAll();

	// search this and children to find the process with given id
	Process* FindProcess(unsigned int n_id);
};

#endif