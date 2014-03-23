#include "GameLogic.h"
#include "Error.h"

void GameLogic::AddObject( GameObject* n_object )
{
	m_objects.push_back(n_object);

	DEBUG_MESSAGE("Added object with ID " << n_object->GetID());
}

void GameLogic::RemoveObject( unsigned int n_id )
{
	for( unsigned int i = 0; i < m_objects.size(); i++ )
	{
		if( m_objects[i]->GetID() == n_id )
		{
			m_objects.erase(m_objects.begin()+i);
			DEBUG_MESSAGE("Removed object with ID " << n_id); 

			return;
		}
	}

	DEBUG_MESSAGE("Weren't able to find and delete object with ID " << n_id);
}

GameObject* GameLogic::FindObject(unsigned int n_id)
{
	for( unsigned int i = 0; i < m_objects.size(); i++ )
	{
		if( m_objects[i]->GetID() == n_id )
			return m_objects[i];
	}
	return NULL;
}

void GameLogic::VUpdate()
{
	for( unsigned int i = 0; i < m_objects.size(); i++ )
		m_objects[i]->VUpdate();
}