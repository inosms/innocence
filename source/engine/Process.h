#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "UniqueNumberGenerator.h"
#include <vector>
#include <thread>



/*
               initialized=true
 ----------|--Init()------------------------End (no method) ----|
 waiting   |     running                                        |
           |                       				   |------------|---dead=true----Deleted
		   |------------------------------Kill()---|
*/

class Process
{
protected:

	// if a process is dead it is removed from the process
	// manager an the m_next Process (if any) can get executed 
	bool m_dead = false;

	// this allows for process chaining
	Process* m_next = nullptr;

	// if the process has already been initialized this
	// is set true
	// if this process is not yet initialized and killed
	// (through recursive call) it is removed immediately
	// without further running
	bool m_initialized = false;

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
	bool IsInitialized();
	void SetInitialized(bool n_init=true);

	Process* GetNext();
	void SetNext( Process* n_process );

	virtual void VUpdate() = 0;
	virtual void VInitialize() = 0;

	// send the process the signal to stop
	// does not have to stop IMMEDIATELY if threaded
	// process; but pls try to do diz as fast as possibru Q.Q
	virtual void VOnRequestKill() = 0;

	// go through this childs and if this or the child with given id is found
	// kill it! WITH FIREEE
	void FindAndKill(unsigned int n_id);

	// kills this and all child processes
	// also deletes uninitialized child processes
	void KillAll();

	// search this and children to find the process with given id
	Process* FindProcess(unsigned int n_id);

	// this process does NOT delete all 
	// m_next processes at destruction
	// as they will likely be executed after it
	virtual ~Process(){}
};



class Thread_Process : public Process
{
private:
	std::thread* m_thread;
protected:
	// if this is set false the process should terminate 
	// as fast as possible!!
	bool m_running = true;
public:

	virtual void VUpdate();
	virtual void VInitialize();
	virtual void VOnRequestKill();
	virtual void VThreadedMethod() = 0;
	virtual ~Thread_Process();
};

#endif