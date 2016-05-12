#pragma once

#include <glm\vec3.hpp>
#include <memory>
#include <vector>

class Shape;
class PhysicsObject;


class PhysicsScene
{
public:
    const static glm::vec3 DefaultGravity;

    void Update(float deltaTime);
    void Draw();

    void AddObject( std::shared_ptr<PhysicsObject> pPhysicsObject);

private:
    void CheckCollisions();

    glm::vec3 m_gravity = DefaultGravity;
    std::vector< std::shared_ptr<PhysicsObject> > m_pPhysicsObjects;

	static constexpr float DampeningCoeffecient = 0.000001f;
};
