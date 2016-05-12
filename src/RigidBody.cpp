#include "RigidBody.h"

glm::vec3 RigidBody::CalculatePositionDelta(float deltaTime, glm::vec3 gravity)
{
    glm::vec3 acceleration = m_force / m_mass;
    acceleration += gravity;
    m_velocity += acceleration * deltaTime;
    glm::vec3 positionDelta = m_velocity * deltaTime;
    m_force = glm::vec3(0);
    return positionDelta;
}

void RigidBody::AddVelocity(glm::vec3 velocity) 
{ 
	m_velocity += velocity; 
}

void RigidBody::AddMomentum(glm::vec3 momentum) 
{ 
	AddVelocity( momentum / m_mass );
}


void RigidBody::AddForce(glm::vec3 force)
{
    m_force += force;
}
