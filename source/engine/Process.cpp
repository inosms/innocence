#include "Process.h"
#include "Error.h"

unsigned int Process::GetID()
{
	return m_id;
}

void Process::SetID(unsigned int n_id)
{
	m_id = n_id;
}

bool Process::IsDead()
{
	return m_dead;
}

bool Process::IsInitialized()
{
	return m_initialized;
}

void Process::SetInitialized(bool n_init)
{
	m_initialized = n_init;
}

Process* Process::GetNext()
{
	return m_next;
}

void Process::SetNext( Process* n_process )
{
	m_next = n_process;
}

void Process::FindAndKill(unsigned int n_id)
{
	if( m_id == n_id )
		VOnRequestKill();
	else if( m_next != nullptr )
		m_next->FindAndKill(n_id);
}

void Process::KillAll()
{
	if( IsInitialized() )
		VOnRequestKill();
	if( m_next != nullptr )
	{
		m_next->KillAll();	

		// this removes all uninitialized children directly
		// it assumes that after one uninitialized process
		// only other uninit. processes follow (otherwise -> memory leak!)
		if( m_next->IsInitialized() == false )
		{
			delete m_next;
			SetNext(nullptr);
		}
	}
}

Process* Process::FindProcess(unsigned int n_id)
{
	if( m_id == n_id )
		return this;
	if( m_next != nullptr )
		return m_next->FindProcess(n_id);

	return nullptr;
}

void Thread_Process::VUpdate()
{
	// do nothing here
}

void Thread_Process::VInitialize()
{
	m_thread = new std::thread([&](){VThreadedMethod();});
}

void Thread_Process::VOnRequestKill()
{
	// this requests that the thread stops running
	// of corse this requires the thread process to 
	// actively use this variable to detect a kill request
	m_running=false;
	try
	{
		// wait for thread to really stop
		m_thread->join();
	}
	catch(...)
	{
		// TODO 
		ERROR_MESSAGE("Hey Exception! No catch block! y u no programm dis?");
	}

	m_dead=true;
}


Thread_Process::~Thread_Process()
{
	delete m_thread;
}