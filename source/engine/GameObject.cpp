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

GameObject::~GameObject()
{
	if( m_body )
		GetGameLogic()->GetPhysics()->DestroyBody(m_body);
}
