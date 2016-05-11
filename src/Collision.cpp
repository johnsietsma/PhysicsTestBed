#include "Collision.h"

#include "PhysicsObject.h"
#include "Shapes.h"


#include <assert.h>
#include <glm\glm.hpp>


const std::array<CollisionDetectionFunction,9> Collision::CollisionDetectionFunctions
{
	PlaneToPlane,	PlaneToSphere,	PlaneToAABB,
	SphereToPlane,	SphereToSphere,	SphereToAABB,
	AABBToPlane,	AABBToSphere,	AABBToAABB
};


bool Collision::Detect(PhysicsObject* pObject1, PhysicsObject* pObject2)
{
	int shapeID1 = pObject1->GetShape()->GetID();
	int shapeID2 = pObject2->GetShape()->GetID();

	// Get the index of the collision function
	int collisionFunctionIndex = shapeID1 * Shape::GetShapeCount() + shapeID2;
	assert(static_cast<size_t>(collisionFunctionIndex) < CollisionDetectionFunctions.size());

	// Call the collison function
	CollisionDetectionFunction collisionFunction = CollisionDetectionFunctions[collisionFunctionIndex];
	if (collisionFunction != nullptr) {
		return collisionFunction(pObject1, pObject2);
	}

	return false;
}

// ----- Plane collisions -----
bool Collision::PlaneToSphere(PhysicsObject* pPlaneObject, PhysicsObject* pSphereObject)
{
	const auto pSphere = pSphereObject->GetShape<Sphere>();
	const auto pPlane = pPlaneObject->GetShape<Plane>();

	const glm::vec3 sphereVector = pSphereObject->GetPosition();
	const glm::vec3 planeNormal = pPlane->GetNormal();

	// Where does the sphere center point overlap the plane normal
	float sphereDistanceAlongPlaneNormal = glm::dot(sphereVector, planeNormal);

	// If the plane distane and sphere radius are bigger then the distance along the normal
	//   then we overlap.
	float overlap = sphereDistanceAlongPlaneNormal - (pPlane->GetDistance() + pSphere->GetRadius());
	if (overlap < 0)
	{
		float totalMass = (pSphereObject->GetMass() + pPlaneObject->GetMass());
		float sphereMassRatio = pSphereObject->GetMass() / totalMass;
		float planeMassRatio = pPlaneObject->GetMass() / totalMass;

		// Separate relative to the mass of the objects
		glm::vec3 separationVector = planeNormal * -overlap;
		pSphereObject->Translate(separationVector * planeMassRatio);
		pPlaneObject->Translate(separationVector * sphereMassRatio);

		return true;
	}

	return false;
}

bool Collision::PlaneToAABB(PhysicsObject* pPlaneObject, PhysicsObject* pAABBObject)
{
	return false;
}

bool Collision::PlaneToPlane(PhysicsObject* pPlaneObject1, PhysicsObject* pPlaneObject2)
{
	return false;
}

// ----- Sphere Collisions ----
bool Collision::SphereToPlane(PhysicsObject* pSphereObject, PhysicsObject* pPlaneObject)
{
	return PlaneToSphere( pPlaneObject, pSphereObject );
}

bool Collision::SphereToSphere(PhysicsObject* pSphereObject1, PhysicsObject* pSphereObject2)
{
	const auto pSphere1 = pSphereObject1->GetShape<Sphere>();
	const auto pSphere2 = pSphereObject1->GetShape<Sphere>();

	float centerDistance = glm::distance(pSphereObject1->GetPosition(), pSphereObject2->GetPosition());
	float radiusDistance = pSphere1->GetRadius() + pSphere2->GetRadius();

	return radiusDistance < centerDistance;
}

bool Collision::SphereToAABB(PhysicsObject* pSphereObject1, PhysicsObject* pAABBObject2)
{
	return false;
}

// ---- AABB Collisions ----
bool Collision::AABBToPlane(PhysicsObject* pAABBObject, PhysicsObject* pPlaneObject)
{
	return PlaneToAABB( pPlaneObject, pAABBObject );
}

bool Collision::AABBToSphere(PhysicsObject* pAABBObject, PhysicsObject* pSphereObject)
{
	return SphereToAABB( pSphereObject, pAABBObject );
}

bool Collision::AABBToAABB(PhysicsObject* pAABBObject1, PhysicsObject* pAABBObject2)
{
	return false;
}
