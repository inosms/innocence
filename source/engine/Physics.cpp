#include "Physics.h"
#include "Application.h"

Physics::Physics()
{
    b2Vec2 tmp_gravity(0.0f,9.81f);
    m_world = new b2World(tmp_gravity);
}

void Physics::Update()
{
    static float tmp_timestep = 1.f/g_application->GetGameLogicTicksDelay();

    // 8 velocity iterations and 3 position iterations are suggested by box2D manual
    m_world->Step(tmp_timestep,8,3);
}

// as the object may have moved during Update()
// this sends all the movement events, in order that
// all the objects in the game represent the changes
void Physics::SendPositionUpdateEvents()
{
    // TODO
}

// create and destroy body; the bodies must be
// saved in the game object and destroyed with the
// game object (thats the job of the game object!)
b2Body* Physics::CreateBody(b2BodyDef& n_def)
{
    return m_world->CreateBody(&n_def);
}

void Physics::DestroyBody(b2Body* n_body)
{
    m_world->DestroyBody(n_body);
}

Physics::~Physics()
{
    delete m_world;
}
