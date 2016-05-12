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

    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetVelocity() const;
	const Shape* GetShape() const { return m_pShape.get(); }
	float GetMass() const;
	glm::vec3 GetMomentum() const { return GetMass() * GetVelocity(); }

	void Translate(glm::vec3 positionDelta);
	void AddVelocity(glm::vec3 velocity) { if (m_pRigidBody != nullptr) m_pRigidBody->AddVelocity(velocity); }
	void AddMomentum(glm::vec3 momentum) { if (m_pRigidBody != nullptr) m_pRigidBody->AddMomentum(momentum); }
	void AddForce(glm::vec3 force) { if (m_pRigidBody != nullptr) m_pRigidBody->AddForce(force); }

    void Update( float deltaTime, glm::vec3 gravity );
    void Draw() { m_pShape->Draw(m_position); };


	void Stop() {
		if (m_pRigidBody != nullptr) m_pRigidBody->Stop();
	}

	template<typename T>
	const T* GetShape() const { return static_cast<const T*>(GetShape()); }

private:
    glm::vec3 m_position;

    std::unique_ptr<Shape> m_pShape;
    std::unique_ptr<RigidBody> m_pRigidBody;
};
