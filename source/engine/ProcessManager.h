#ifndef _PROCESSMANAGER_H_
#define _PROCESSMANAGER_H_

#include "Process.h"
#include <vector>

class ProcessManager
{

	std::vector<Process*> m_processes;

	// counter for unique id given, which are given to
	// the processes when added
	// this should not wrap around so fast
	// but still: FIXME 
	unsigned int m_nextID = 1;

	// deletes a process from internal process list
	// doesn't care if killed/dead or not
	// if the deleted process has some (non-dead) children
	// those children are now allowed to run
	// ONLY FOR INTERNAL USE
	void DeleteProcess(unsigned int n_id);

public:
	// Adds a Process and returns a unique identifier by which the process can be killed again
	unsigned int AddProcess( Process* n_process );

	// Kill process with given id and (if any) 
	// mark the following process to be executed
	void KillProcess( unsigned int n_id );

	// Kills the process with given id and all following processes
	void KillProcessWithChildren( unsigned int n_id );

	void KillAllProcesses();
	bool AllProcessesDead();

	Process* FindProcess( unsigned int n_id );

	// update all processes 
	// (this will most likely have no 
	// effect for threaded processes)
	void Update();
};

#endif