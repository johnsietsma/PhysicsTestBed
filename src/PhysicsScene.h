#pragma once

#include <glm\vec3.hpp>
#include <memory>
#include <vector>

class Shape;

class PhysicsObject;
typedef bool(*CollisionDetectionFunction)(Shape* pShape1, Shape* pShape2);

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


    static CollisionDetectionFunction collisionDetectionFunctions[];
};
