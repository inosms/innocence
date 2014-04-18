#include "ProcessManager.h"
#include "Error.h"

unsigned int ProcessManager::AddProcess( Process* n_process )
{
	m_processes.push_back(n_process);
	n_process->SetID(m_nextID);

	return m_nextID++;
}

void ProcessManager::KillProcess( unsigned int n_id )
{
	for( Process* i_process : m_processes )
		i_process->FindAndKill(n_id);
}

void ProcessManager::KillProcessWithChildren( unsigned int n_id )
{
	Process* tmp_toKill = FindProcess(n_id);

	if( tmp_toKill = nullptr )
		ERROR_MESSAGE("Process with ID " << n_id << " was not found");
	else
		// kills this process and all connected child processes
		tmp_toKill->KillAll();
}

void ProcessManager::KillAllProcesses()
{
	for( Process* i_process : m_processes )
		i_process->KillAll();
}

bool ProcessManager::AllProcessesDead()
{
	if( m_processes.empty() ) return true;

	// check if non dead processes are in list
	for( Process* i_process : m_processes )
	{
		// if one is not dead, abort with return false
		if( i_process->IsDead() == false )
			return false;
		// check if a chained process which is not dead is existing
		else if( 	i_process->IsDead() == true &&
					i_process->GetNext() != nullptr &&
					i_process->GetNext()->IsDead() == false )
			return false;
	}

	// everyting is DEEAAD
	return true;
}

Process* ProcessManager::FindProcess( unsigned int n_id )
{
	for( Process* i_process : m_processes )
	{
		Process* tmp_find = i_process->FindProcess(n_id);
		if( tmp_find != nullptr )
			return tmp_find;
	}
	return nullptr;
}

void ProcessManager::Update()
{
	// TODO 
}