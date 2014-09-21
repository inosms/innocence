#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include "GameObject.h"
#include "EventListener.h"
#include "Process.h"
#include <vector>

enum GameLogicState
{
	GameLogicState_Running,
	GameLogicState_Loading
};

class GameLogic
{
protected:
	std::vector<GameObject*> m_objects;
	EventListener* m_listener;

	GameLogicState m_state = GameLogicState_Running;
public:

	GameLogic( EventListener* n_listener );

	void AddObject( GameObject* n_object );
	void RemoveObject( unsigned int n_id );

	// returns Object with n_id as ID;
	// return NULL if not found
	GameObject* FindObject(unsigned int n_id);

	void SetState(int n_state);

	virtual void VUpdate();
};

#endif
