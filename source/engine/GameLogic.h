#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include "GameObject.h"
#include <vector>

class GameLogic
{
protected:
	std::vector<GameObject*> m_objects;

public:
	void AddObject( GameObject* n_object );
	void RemoveObject( unsigned int n_id );

	// returns Object with n_id as ID; 
	// return NULL if not found
	GameObject* FindObject(unsigned int n_id);

	virtual void VUpdate();
};

#endif