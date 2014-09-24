#include "inGameObject.h"
#include "inSceneNode.h"

inCreator::inCreator() : Creator()
{}


inCreator* TypeToCreator(int n_type)
{
	if( n_type == inGameObject_Type_Test )
		return new Creator_Test();
	// always TODO
	else
		return nullptr;
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
	GameObject_Test* tmp_test = new GameObject_Test(m_id);
	n_gameLogic.AddObject(tmp_test);
}

void Creator_Test::CreateSceneNode(GameView_Human& n_gameView)
{
	SceneNode_Test* tmp_test = new SceneNode_Test(m_id,m_width);
	n_gameView.GetScene()->AddSceneNode(tmp_test);
}

void Creator_Test::VLoad(pugi::xml_node n_node)
{
	m_width = toInt(n_node.child_value("width"));
}
