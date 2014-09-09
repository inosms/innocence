#include "GameLogic.h"
#include "Error.h"

GameLogic::GameLogic(EventListener* n_listener) :
	m_listener(n_listener){}

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
			delete m_objects[i];
			m_objects.erase(m_objects.begin()+i);
			DEBUG_MESSAGE("Removed object with ID " << n_id);

			return;
		}
	}

	DEBUG_MESSAGE("Weren't able to find and delete object with ID " << n_id);
}

GameObject* GameLogic::FindObject(unsigned int n_id)
{
	for( GameObject* i_gameObject : m_objects )
	{
		if( i_gameObject->GetID() == n_id )
			return i_gameObject;
	}
	return nullptr;
}

void GameLogic::VUpdate()
{
	for( GameObject* i_gameObject : m_objects )
		i_gameObject->VUpdate();

	m_listener->ProcessQueuedEvents();
}
