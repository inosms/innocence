#include "Process.h"

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
	VOnRequestKill();
	if( m_next != nullptr )
		m_next->KillAll();	
}

Process* Process::FindProcess(unsigned int n_id)
{
	if( m_id == n_id )
		return this;
	if( m_next != nullptr )
		return m_next->FindProcess(n_id);

	return nullptr;
}