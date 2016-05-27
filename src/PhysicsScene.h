#pragma once

#include "BasePhysicsScene.h"

#include <memory>
#include <vector>

class Shape;
class PhysicsObject;
class RigidBody;


class PhysicsScene : public BasePhysicsScene
{
public:
	PhysicsScene() : PhysicsScene(glm::vec3(0)) {}
	PhysicsScene(glm::vec3 offset) : m_offset(offset) {}

    void Update(float deltaTime) override;
    void Draw() override;

	void AddPlaneStatic(glm::vec3 normal, float distance) override;
	void AddSphereStatic(glm::vec3 position, float radius) override;
	void AddAABBStatic(glm::vec3 position, glm::vec3 extents) override;

	void AddPlaneDynamic(glm::vec3 normal, float distance, float mass, glm::vec3 velocity) override;
	void AddSphereDynamic(glm::vec3 position, float radius, float mass, glm::vec3 velocity) override;
	void AddAABBDynamic(glm::vec3 position, glm::vec3 extents, float mass, glm::vec3 velocity) override;

private:
	void AddPlane(glm::vec3 normal, float distance, RigidBody* pRigidBody=nullptr);
	void AddSphere(glm::vec3 position, float radius, RigidBody* pRigidBody=nullptr);
	void AddAABB(glm::vec3 position, glm::vec3 extents, RigidBody* pRigidBody=nullptr);

	void AddObject(std::shared_ptr<PhysicsObject> pPhysicsObject);
    void CheckCollisions();

	glm::vec3 m_offset;
    glm::vec3 m_gravity = DefaultGravity;
    std::vector< std::shared_ptr<PhysicsObject> > m_pPhysicsObjects;
};
