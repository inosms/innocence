#include "Physics.h"
#include "Application.h"
#include "Event.h"
#include "EventManager.h"
#include "Math.h"

Physics::Physics()
{
    b2Vec2 tmp_gravity(0.0f,-9.81f);
    m_world = new b2World(tmp_gravity);
}

void Physics::Update()
{
    static float tmp_timestep = 1.f/(1000.f/g_application->GetGameLogicTicksDelay());

    // 8 velocity iterations and 3 position iterations are suggested by box2D manual
    m_world->Step(tmp_timestep,8,3);
}

// as the object may have moved during Update()
// this sends all the movement events, in order that
// all the objects in the game represent the changes
void Physics::SendPositionUpdateEvents()
{
    for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
    {
        unsigned int tmp_id = *((unsigned int*) b->GetUserData());
        b2Vec2 tmp_pos = b->GetPosition();
        glm::mat4x4 tmp_newPos = glm::translate(glm::mat4(1.f),glm::vec3(tmp_pos.x,tmp_pos.y,0.f));
        SEND_EVENT(Event_MoveObject,tmp_newPos,tmp_id);
    }
}

// create and destroy body; the bodies must be
// saved in the game object and destroyed with the
// game object (thats the job of the game object!)
b2Body* Physics::CreateBody(b2BodyDef& n_def, unsigned int n_id)
{
    // save the id with the object
    n_def.userData = new unsigned int(n_id);

    return m_world->CreateBody(&n_def);
}

void Physics::DestroyBody(b2Body* n_body)
{
    // destroy the saved id
    delete (unsigned int *) n_body->GetUserData();
    m_world->DestroyBody(n_body);
}

Physics::~Physics()
{
    delete m_world;
}
