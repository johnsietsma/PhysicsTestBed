#pragma once

#include <array>

class PhysicsObject;

typedef bool(*CollisionDetectionFunction)(PhysicsObject* pShape1, PhysicsObject* pShape2);


class Collision
{
    Collision() = delete;
public:
	static bool Detect(PhysicsObject* pObject1, PhysicsObject* pObject2);


private:
	static const std::array<CollisionDetectionFunction, 9> CollisionDetectionFunctions;

	// Plane Collisions
	static bool PlaneToSphere(PhysicsObject* pPlaneObject, PhysicsObject* pSphereObject);
	static bool PlaneToAABB(PhysicsObject* pPlaneObject, PhysicsObject* pAABBObject);
	static bool PlaneToPlane(PhysicsObject* pPlaneObject1, PhysicsObject* pPlaneObject2);

	// Sphere Collisions
	static bool SphereToPlane(PhysicsObject* pSphereObject, PhysicsObject* pPlaneObject);
	static bool SphereToSphere(PhysicsObject* pSphereObject1, PhysicsObject* pSphereObject2);
	static bool SphereToAABB(PhysicsObject* pSphereObject, PhysicsObject* pAABBObject);

	// AABB Collisions
	static bool AABBToPlane(PhysicsObject* pAABBObject, PhysicsObject* pPlaneObject);
	static bool AABBToSphere(PhysicsObject* pAABBObject, PhysicsObject* pSphereObject);
	static bool AABBToAABB(PhysicsObject* pAABBObject1, PhysicsObject* pAABBObject2);

};
