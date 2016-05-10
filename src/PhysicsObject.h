#pragma once

#include "RigidBody.h"
#include "Shapes.h"

#include <memory>
#include <glm\vec3.hpp>

class PhysicsObject
{
public:
    PhysicsObject(glm::vec3 initialPosition, Shape* pShape, RigidBody* pRigidBody = nullptr) :
        m_position(initialPosition),
        m_pShape(pShape),
        m_pRigidBody(pRigidBody)
    {}

    void Update( float deltaTime, glm::vec3 gravity );
    void Draw() { m_pShape->Draw(m_position); };

private:
    glm::vec3 m_position;

    std::unique_ptr<Shape> m_pShape;
    std::unique_ptr<RigidBody> m_pRigidBody;
};
