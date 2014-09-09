#include "ProcessManager.h"
#include "Error.h"

void ProcessManager::DeleteProcess(unsigned int n_id)
{
	DEBUG_MESSAGE("Delete process " << n_id);
	for( unsigned int i = 0; i < m_processes.size(); i++ )
	{
		if( m_processes[i]->GetID() == n_id )
		{
			Process* tmp_next = m_processes[i]->GetNext();

			delete m_processes[i];
			
			// if there was a child attach the child to the process manager
			// if not delete the entry
			if( tmp_next != nullptr )
			{
				m_processes[i] = tmp_next;
			}
			else
			{
				m_processes.erase(m_processes.begin()+i);
				// no need for further search
				return;
			}
		}
	}
}

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

	if( tmp_toKill == nullptr )
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
	// idea: 
	// if all processes are dead, the list is emptied;
	// the list is filled with processes as long as they
	// run or the list is not updated
	// thus while waiting for all processes to die
	// the processmanager should be updated until
	// this method returns true
	return m_processes.empty(); 
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
	DEBUG_MESSAGE("ProcessManager Update");

	std::vector<unsigned int> tmp_idsToDelete;

	for( Process* i_process : m_processes )
	{
		// only execute living processes
		if( i_process->IsDead() == false )
		{
			
			if( i_process->IsInitialized() == false )
			{
				i_process->VInitialize();
				i_process->SetInitialized(true);
			}

			i_process->VUpdate();
		}
		else
			tmp_idsToDelete.push_back( i_process->GetID() );
	}

	//DEBUG_MESSAGE("attempt deleting");
	for( unsigned int i_id : tmp_idsToDelete )
	{
		DeleteProcess(i_id);
	}
	//DEBUG_MESSAGE("success deleting");
}