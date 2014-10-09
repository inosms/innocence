#include "GameObject.h"
#include "Application.h"

GameObject::GameObject(unsigned int n_id, unsigned int n_type)
	: 	m_id(n_id),
		m_type(n_type)
		{}

unsigned int GameObject::GetID()
{
	return m_id;
}

unsigned int GameObject::GetType()
{
	return m_type;
}

glm::mat4x4 GameObject::GetPos()
{
	return m_pos;
}

void GameObject::SetPos(glm::mat4x4 n_pos)
{
	m_pos = n_pos;
}

b2Body* GameObject::GetBody()
{
	return m_body;
}

void GameObject::SetBody(b2Body* n_body)
{
	m_body = n_body;
}

GameObject::~GameObject()
{
	if( m_body )
		GetGameLogic().GetPhysics()->DestroyBody(m_body);
}
