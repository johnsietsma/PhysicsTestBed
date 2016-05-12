#pragma once

#include <glm\vec3.hpp>

class RigidBody
{
public:
    RigidBody(float mass) :
        m_mass(mass)
    {}

    float GetMass() const { return m_mass; }
    glm::vec3 GetVelocity() const { return m_velocity;  }

    glm::vec3 CalculatePositionDelta(float deltaTime, glm::vec3 gravity);

	void Stop() { m_velocity = glm::vec3(0); }
    void AddVelocity(glm::vec3 velocity) { m_velocity += velocity; }
    void AddForce(glm::vec3 force);

private:
    // Constants
    float m_mass;

    // Derived data
    glm::vec3 m_velocity;

    // Accumulated data
    glm::vec3 m_force;
};