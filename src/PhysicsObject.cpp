#include "PhysicsObject.h"

#include "RigidBody.h"

#include <limits>


glm::vec3 PhysicsObject::GetVelocity() const
{
    return m_pRigidBody == nullptr ? glm::vec3(0) : m_pRigidBody->GetVelocity();
}


float PhysicsObject::GetMass() const
{
	if( m_pRigidBody == nullptr ) {
		// Model a static object as an object with a very large mass
		return std::numeric_limits<float>::max();
	}
	else {
		return m_pRigidBody->GetMass();
	}
}

void PhysicsObject::Update(float deltaTime, glm::vec3 gravity)
{
    if (m_pRigidBody != nullptr) {
        m_position += m_pRigidBody->CalculatePositionDelta(deltaTime, gravity);
    }
}
