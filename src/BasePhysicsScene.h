#pragma once

#include <glm/vec3.hpp>

class BasePhysicsScene
{
public:
	virtual ~BasePhysicsScene() {}

    const static glm::vec3 DefaultGravity;

	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	virtual void AddPlaneStatic(glm::vec3 normal, float distance) = 0;
	virtual void AddSphereStatic(glm::vec3 position, float radius) = 0;
	virtual void AddAABBStatic(glm::vec3 position, glm::vec3 extents) = 0;

	virtual void AddPlaneDynamic(glm::vec3 normal, float distance, float mass, glm::vec3 velocity) = 0;
	virtual void AddSphereDynamic(glm::vec3 position, float radius, float mass, glm::vec3 velocity) = 0;
	virtual void AddAABBDynamic(glm::vec3 position, glm::vec3 extents, float mass, glm::vec3 velocity) = 0;
};
