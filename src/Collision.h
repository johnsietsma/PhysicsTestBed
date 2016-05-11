#pragma once

#include <array>

class PhysicsObject;

typedef bool(*CollisionDetectionFunction)(PhysicsObject* pShape1, PhysicsObject* pShape2);


class Collision
{
    Collision() = delete;
public:
	static bool Detect(PhysicsObject* pObject1, PhysicsObject* pObject2);

	static const std::array<CollisionDetectionFunction,9> CollisionDetectionFunctions;



    static bool SphereToSphere(PhysicsObject* pSphereObject1, PhysicsObject* pSphereObject2);
    static bool SphereToPlane(PhysicsObject* pSphereObject, PhysicsObject* pPlaneObject);
    static bool PlaneToSphere(PhysicsObject* pPlaneObject, PhysicsObject* pSphereObject);
};
