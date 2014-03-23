#ifndef _CREATOR_H_
#define _CREATOR_H_

class GameLogic;
class GameView;

class Creator
{
protected:
	unsigned int m_id;

public:

	// automatically creates a new ID
	Creator();

	// should create a new game object, which should be added to 
	// the game logic afterwards -  and must also set up the object
	// (also for physicssystem etc; as this is _not_ handled automatically)
	virtual void CreateGameObject(GameLogic* n_gameLogic) = 0;

	// similar to CreateGameObject this must set up the SceneNode
	// and add it to the GameView
	virtual void CreateScenenNode(GameView* n_gameView) = 0;
};

#endif