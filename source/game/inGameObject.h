#ifndef _INGAMEOBJECT_H_
#define _INGAMEOBJECT_H_

#include "../engine/engine.h"

enum inGameObject_Type
{
	inGameObject_Type_Test
};

class GameObject_Test : public GameObject
{
public:
	GameObject_Test(unsigned int n_id);
	virtual void VUpdate();

};

class Creator_Test : public Creator
{
public:
	virtual void CreateGameObject(GameLogic& n_gameLogic);
	virtual void CreateSceneNode(GameView_Human& n_gameView);
};

#endif