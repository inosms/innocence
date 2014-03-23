#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

class GameObject
{
protected:
	unsigned int m_id;
	unsigned int m_type;

public:

	GameObject(unsigned int n_id, unsigned int n_type);

	unsigned int GetID();
	unsigned int GetType();
	
	virtual void VUpdate() = 0;

};

#endif