#include "inGameObject.h"
#include "inSceneNode.h"

inCreator::inCreator() : Creator()
{}

void inCreator::VLoad(pugi::xml_node n_node)
{
	pugi::xml_node tmp_position = n_node.child("position");
	m_position.x = toFloat(tmp_position.child_value("x"));
	m_position.y = toFloat(tmp_position.child_value("y"));
	m_position.z = toFloat(tmp_position.child_value("z"));
}


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
}

void Creator_Test::CreateGameObject(GameLogic& n_gameLogic)
{
	GameObject_Test* tmp_test = new GameObject_Test(m_id);
	n_gameLogic.AddObject(tmp_test);
}

void Creator_Test::CreateSceneNode(GameView_Human& n_gameView)
{
	SceneNode_Test* tmp_test = new SceneNode_Test(m_id,m_width,m_height);
	glm::mat4 tmp_translate = glm::translate(glm::mat4(1.f),m_position);
	tmp_test->SetNewMatrixNoInterpolation(tmp_translate);
	n_gameView.GetScene()->AddSceneNode(tmp_test);
}

void Creator_Test::VLoad(pugi::xml_node n_node)
{
	inCreator::VLoad(n_node);
	m_width = toFloat(n_node.child_value("width"));
	m_height = toFloat(n_node.child_value("height"));

}
