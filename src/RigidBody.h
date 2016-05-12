#pragma once

#include <glm\vec3.hpp>

class RigidBody
{
public:
    RigidBody(float mass, glm::vec3 initialVelocity) :
        m_mass(mass),
		m_velocity(initialVelocity)
    {}

    float GetMass() const { return m_mass; }
    glm::vec3 GetVelocity() const { return m_velocity;  }
	glm::vec3 GetMomentum() const { return GetMass() * GetVelocity(); }

    glm::vec3 CalculatePositionDelta(float deltaTime, glm::vec3 gravity);

	void Stop() { m_velocity = glm::vec3(0); }
	void AddVelocity(glm::vec3 velocity);
	void AddMomentum(glm::vec3 momentum);
    void AddForce(glm::vec3 force);

private:
    // Constants
    float m_mass;

    // Derived data
    glm::vec3 m_velocity;

    // Accumulated data
    glm::vec3 m_force;
};