#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Math.h"

class b2Body;
class GameObject
{
protected:
	unsigned int m_id;
	unsigned int m_type;
	glm::mat4x4 m_pos;

	// every game object can have a physical body
	b2Body* m_body = nullptr;
public:

	GameObject(unsigned int n_id, unsigned int n_type);

	unsigned int GetID();
	unsigned int GetType();

	glm::mat4x4 GetPos();
	void SetPos(glm::mat4x4 n_pos);

	b2Body* GetBody();
	void SetBody(b2Body* n_body);

	virtual void VUpdate() = 0;

	virtual ~GameObject();

};

#endif
