#ifndef _INGAMEOBJECT_H_
#define _INGAMEOBJECT_H_

#include "../engine/engine.h"

enum inGameObject_Type
{
	inGameObject_Type_Test
};

/*

TODO: this maybe later?
for now: load all objects with begin
load all resources whith the objects
-> keep levels small
enum inCreatorResourceType
{
	inCreatorResourceType_Texture,
	inCreatorResourceType_Mesh,
	// todo: fragment? vertex? there is only one std::string!
	inCreatorResourceType_Shader
};
*/

class inCreator : public Creator
{
protected:
	// some default values
	glm::vec3 m_position;

public:
	inCreator();
//	virtual std::vector< std::pair<inCreatorResourceType, std::string> > VGetNeededResources() = 0;

	// called when level is loaded with the
	// node which causes this Creator to be created
	// this is just a default implementation
	virtual void VLoad(pugi::xml_node n_node);
};



// used for loading: given a type it resturns the correct inCreator
// MUST BE EXTENDED FOR EVERY NEW CREATOR!!!!
extern inCreator* TypeToCreator(int n_type);

class GameObject_Test : public GameObject
{
public:
	GameObject_Test(unsigned int n_id);
	virtual void VUpdate();

};

class Creator_Test : public inCreator
{
	float m_width = 1, m_height = 1;
public:
	virtual void CreateGameObject(GameLogic& n_gameLogic);
	virtual void CreateSceneNode(GameView_Human& n_gameView);
	virtual void VLoad(pugi::xml_node n_node);
};

#endif
