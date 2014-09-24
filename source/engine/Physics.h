#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "Box2D/Box2D/Box2D.h"

class Physics
{
    b2World* m_world;

public:
    Physics();

    // updates the game world
    void Update();

    // as the object may have moved during Update()
    // this sends all the movement events, in order that
    // all the objects in the game represent the changes
    void SendPositionUpdateEvents();

    // create and destroy body; the bodies must be
    // saved in the game object and destroyed with the
    // game object (thats the job of the game object!)
    b2Body* CreateBody(b2BodyDef& n_def);
    void DestroyBody(b2Body* n_body);

    ~Physics();
};

#endif
