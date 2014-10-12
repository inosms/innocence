#include "GameLogic.h"
#include "Error.h"
#include "engine.h"

GameLogic::GameLogic(EventListener* n_listener) :
	m_listener(n_listener)
	{
		// add all default event types
		FORALLDEFAULTEVENTTYPES(GetEventManager().AddEventListener(m_listener,event));
	}

GameLogic::~GameLogic()
{}

void GameLogic::AddObject( GameObject* n_object )
{
	m_objects.push_back(std::shared_ptr<GameObject>(n_object));

	// sort; as finding will require a sorted set
	std::sort(m_objects.begin(),m_objects.end(),
		[](const std::shared_ptr<GameObject> & a, const std::shared_ptr<GameObject> & b)->bool
		{
			return a->GetID() < b->GetID();
		});

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

std::weak_ptr<GameObject> GameLogic::FindObject(unsigned int n_id)
{
	// find object by binary search
	auto tmp_found = std::lower_bound(m_objects.begin(),m_objects.end(),n_id,
		[](const std::shared_ptr<GameObject> & a, const unsigned int & b)->bool
		{
			 return a->GetID() < b;
		});

	// if it is not found or the found one is not n_id (because lower_bound)
	if( tmp_found == m_objects.end() || (*tmp_found)->GetID() != n_id)
		return std::weak_ptr<GameObject>();
	else
		return std::weak_ptr<GameObject>(*tmp_found);
}

void GameLogic::SetState(int n_state)
{
	m_state = (GameLogicState)n_state;
}

void GameLogic::Exit()
{
	// remove all game objects
	// if it is done in constructor it causes great chaos
	// as the physics system is deleted before
	// and then the objects can not be deleted from it
	while(m_objects.size()>0)
		m_objects.erase(m_objects.begin());
}

Physics* GameLogic::GetPhysics()
{
	return &m_physics;
}


void GameLogic::VUpdate()
{
	// if loading do not update the game objects
	if( m_state == GameLogicState_Running )
	{
		for( auto& i_gameObject : m_objects )
			i_gameObject->VUpdate();

		m_physics.Update();
		m_physics.SendPositionUpdateEvents();
	}

	// always process events in every state
	m_listener->ProcessQueuedEvents();
}
