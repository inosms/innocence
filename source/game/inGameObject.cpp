#include "inGameObject.h"
#include "inSceneNode.h"

inCreator::inCreator() : Creator()
{}

void inCreator::SetDefaultValues(GameObject& n_obj)
{
	glm::mat4 tmp_translate = glm::translate(glm::mat4(1.f),m_position);
	n_obj.SetPos(tmp_translate);
}

void inCreator::SetDefaultValues(SceneNode& n_node)
{
	glm::mat4 tmp_translate = glm::translate(glm::mat4(1.f),m_position);
	n_node.SetNewMatrixNoInterpolation(tmp_translate);
}


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
	else if( n_type == inGameObject_Type_DynamicTest )
		return new Creator_DynamicTest();
	else if( n_type == inGameObject_Type_PlayerTest )
		return new Creator_PlayerTest();
	// always TODO
	else
	{
		ERROR_MESSAGE("trying to create Creator with unknown type!");
		return nullptr;
	}
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
	SetDefaultValues(*tmp_test);

	b2BodyDef tmp_def;
	tmp_def.type = b2_staticBody;
	tmp_def.position.Set(m_position.x,m_position.y);
	b2Body* tmp_body = n_gameLogic.GetPhysics()->CreateBody(tmp_def,m_id);
	tmp_test->SetBody(tmp_body);

	b2PolygonShape tmp_polygon;
	tmp_polygon.SetAsBox(m_width/2.f,m_height/2.f);
	tmp_body->CreateFixture(&tmp_polygon,0.f);

	n_gameLogic.AddObject(tmp_test);
}

void Creator_Test::CreateSceneNode(GameView_Human& n_gameView)
{
	SceneNode_Test* tmp_test = new SceneNode_Test(m_id,m_width,m_height);
	SetDefaultValues(*tmp_test);
	n_gameView.GetScene()->AddSceneNode(tmp_test);
}

void Creator_Test::VLoad(pugi::xml_node n_node)
{
	inCreator::VLoad(n_node);
	m_width = toFloat(n_node.child_value("width"));
	m_height = toFloat(n_node.child_value("height"));
}


GameObject_DynamicTest::GameObject_DynamicTest(unsigned int n_id) :
	GameObject(n_id,inGameObject_Type_DynamicTest)
{}

void GameObject_DynamicTest::VUpdate(){}


void Creator_DynamicTest::CreateGameObject(GameLogic& n_gameLogic)
{
	GameObject_DynamicTest* tmp_test = new GameObject_DynamicTest(m_id);
	SetDefaultValues(*tmp_test);

	b2BodyDef tmp_def;
	tmp_def.type = b2_dynamicBody;
	tmp_def.position.Set(m_position.x,m_position.y);
	b2Body* tmp_body = n_gameLogic.GetPhysics()->CreateBody(tmp_def,m_id);
	tmp_test->SetBody(tmp_body);

	b2PolygonShape tmp_polygon;
	tmp_polygon.SetAsBox(0.2,0.2);
	b2FixtureDef tmp_fixture;
	tmp_fixture.shape = &tmp_polygon;
	tmp_fixture.density = 1.0f;
	tmp_fixture.friction = 0.3f;

	tmp_body->CreateFixture(&tmp_fixture);

	n_gameLogic.AddObject(tmp_test);
}

void Creator_DynamicTest::CreateSceneNode(GameView_Human& n_gameView)
{
	SceneNode_Test* tmp_test = new SceneNode_Test(m_id,0.4,0.4);
	SetDefaultValues(*tmp_test);
	n_gameView.GetScene()->AddSceneNode(tmp_test);
}



GameObject_PlayerTest::GameObject_PlayerTest(unsigned int n_id) : GameObject(inGameObject_Type_PlayerTest,n_id)
{}

void GameObject_PlayerTest::VUpdate(){}

void GameObject_PlayerTest::OnRequestPlayerMove(glm::vec2 n_direction)
{
	b2Vec2 tmp_dir(n_direction.x,n_direction.y);
	b2Vec2 tmp_endVel = 10.0 * tmp_dir;
	b2Vec2 tmp_nowVel = m_body->GetLinearVelocity();
	b2Vec2 tmp_diffVel = tmp_endVel - tmp_nowVel;
	b2Vec2 tmp_impulse = m_body->GetMass() * tmp_diffVel;
	m_body->ApplyLinearImpulse(tmp_impulse,m_body->GetWorldCenter(),true);
}

void GameObject_PlayerTest::OnRequestPlayerStop()
{
	b2Vec2 tmp_nowVel = m_body->GetLinearVelocity();
	// apply the negative of current velocity to stop the movement
	b2Vec2 tmp_impulse = m_body->GetMass() * -1 * tmp_nowVel;
	m_body->ApplyLinearImpulse(tmp_impulse,m_body->GetWorldCenter(),true);
}

void GameObject_PlayerTest::OnRequestPlayerJump()
{
	m_body->ApplyLinearImpulse(b2Vec2(0.f,20.f),m_body->GetWorldCenter(),true);
}

void Creator_PlayerTest::CreateGameObject(GameLogic& n_gameLogic)
{
	GameObject_PlayerTest* tmp_test = new GameObject_PlayerTest(m_id);
	SetDefaultValues(*tmp_test);

	b2BodyDef tmp_def;
	tmp_def.type = b2_dynamicBody;
	tmp_def.fixedRotation = true;
	tmp_def.position.Set(m_position.x,m_position.y);
	b2Body* tmp_body = n_gameLogic.GetPhysics()->CreateBody(tmp_def,m_id);
	tmp_test->SetBody(tmp_body);

	b2PolygonShape tmp_polygon;
	tmp_polygon.SetAsBox(0.5,1.8);
	b2FixtureDef tmp_fixture;
	tmp_fixture.shape = &tmp_polygon;
	tmp_fixture.density = 1.0f;
	tmp_fixture.friction = 0.3f;

	tmp_body->CreateFixture(&tmp_fixture);

	n_gameLogic.AddObject(tmp_test);
}
void Creator_PlayerTest::CreateSceneNode(GameView_Human& n_gameView)
{
	SceneNode_Test* tmp_test = new SceneNode_Test(m_id,1,1.8*2);
	SetDefaultValues(*tmp_test);
	n_gameView.GetScene()->AddSceneNode(tmp_test);
}
