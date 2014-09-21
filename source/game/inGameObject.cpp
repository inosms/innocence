#include "inGameObject.h"
#include "inSceneNode.h"


inCreator* TypeToCreator(inGameObject_Type n_type)
{
	// TODO!!!!!
}


GameObject_Test::GameObject_Test(unsigned int n_id) : GameObject(n_id,inGameObject_Type_Test)
{

}

void GameObject_Test::VUpdate()
{
	std::cout << "test" << std::endl;
}

void Creator_Test::CreateGameObject(GameLogic& n_gameLogic)
{
	// TODO 
}

void Creator_Test::CreateSceneNode(GameView_Human& n_gameView)
{
	SceneNode_Test* tmp_test = new SceneNode_Test(m_id);
	n_gameView.GetScene()->AddSceneNode(tmp_test);
}