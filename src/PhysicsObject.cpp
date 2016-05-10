#include "PhysicsObject.h"

#include "RigidBody.h"

void PhysicsObject::Update(float deltaTime, glm::vec3 gravity)
{
    if (m_pRigidBody != nullptr) {
        m_position += m_pRigidBody->CalculatePositionDelta(deltaTime, gravity);
    }
}
