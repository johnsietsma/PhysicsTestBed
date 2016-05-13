#include "RigidBody.h"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

glm::vec3 RigidBody::CalculatePositionDelta(float deltaTime, glm::vec3 gravity)
{
    glm::vec3 acceleration = m_force / m_mass;
    glm::vec3 oldVelocity = m_velocity;
    m_velocity += acceleration * deltaTime;
    m_velocity += gravity * deltaTime;

    // See for a good overview of integration techniques
    // https://jdickinsongames.wordpress.com/2015/01/22/numerical-integration-in-games-development-2/

    // Midpoint method
    glm::vec3 positionDelta = (oldVelocity+m_velocity)*0.5f * deltaTime;

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
