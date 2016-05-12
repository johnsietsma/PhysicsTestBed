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


void Separate( PhysicsObject* pObject1, PhysicsObject* pObject2, float overlap, glm::vec3 normal )
{
	float totalMass = pObject1->GetMass() + pObject2->GetMass();
	float massRatio1 = pObject1->GetMass() / totalMass;
	float massRatio2 = pObject2->GetMass() / totalMass;

	// Separate relative to the mass of the objects
	glm::vec3 separationVector = normal * overlap;
	pObject1->Translate(-separationVector * massRatio2);
	pObject2->Translate(separationVector * massRatio1);
}

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


// ---- Point Collisions ----
bool Collision::PointToPlane(glm::vec3 point, const Plane* pPlane)
{
	return glm::dot(point, pPlane->GetNormal()) < 0;
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
    overlap *= -1;
	if (overlap > 0)
	{
		Separate(pPlaneObject, pSphereObject, overlap, planeNormal);
		return true;
	}

	return false;
}

bool Collision::PlaneToAABB(PhysicsObject* pPlaneObject, PhysicsObject* pAABBObject)
{
	const auto pPlane = pPlaneObject->GetShape<Plane>();
	const auto pAABB = pAABBObject->GetShape<AABB>();

	glm::vec3 AABBPos = pAABBObject->GetPosition();
	glm::vec3 minPos = AABBPos - pAABB->GetExtents();
	glm::vec3 maxPos = AABBPos + pAABB->GetExtents();

	float minPointDistanceAlongPlaneNormal = glm::dot(minPos, pPlane->GetNormal());
	float maxPointDistanceAlongPlaneNormal = glm::dot(maxPos, pPlane->GetNormal());

	float overlap = -std::min(minPointDistanceAlongPlaneNormal, maxPointDistanceAlongPlaneNormal);

	if(overlap > 0 ) {
		Separate(pPlaneObject, pAABBObject, overlap, pPlane->GetNormal());
		return true;
	}

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

bool Collision::SphereToAABB(PhysicsObject* pSphereObject, PhysicsObject* pAABBObject)
{
    const auto pSphere = pSphereObject->GetShape<Sphere>();
    const auto pAABB = pAABBObject->GetShape<AABB>();

    glm::vec3 minPos = -pAABB->GetExtents();
    glm::vec3 maxPos = pAABB->GetExtents();

    glm::vec3 clampedPoint(0);
    glm::vec3 distance = pSphereObject->GetPosition() - pAABBObject->GetPosition();

    if (distance.x < minPos.x)
        clampedPoint.x = minPos.x;
    else if (distance.x > maxPos.x)
        clampedPoint.x = maxPos.x;
    else
        clampedPoint.x = distance.x;

    if (distance.y < minPos.y)
        clampedPoint.y = minPos.y;
    else if (distance.y > maxPos.y)
        clampedPoint.y = maxPos.y;
    else
        clampedPoint.y = distance.y;

    if (distance.z < minPos.z)
        clampedPoint.z = minPos.z;
    else if (distance.z > maxPos.z)
        clampedPoint.z = maxPos.z;
    else
        clampedPoint.z = distance.z;

    glm::vec3 clampedDistance = distance - clampedPoint;

    float overlap = glm::length(clampedDistance) - pSphere->GetRadius();
    overlap *= -1;
    if (overlap > 0)
    {
        Separate(pAABBObject, pSphereObject, overlap, glm::normalize(clampedDistance));
        return true;
    }

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
    const auto pAABB1 = pAABBObject1->GetShape<AABB>();
    const auto pAABB2 = pAABBObject2->GetShape<AABB>();

    glm::vec3 box1Pos = pAABBObject1->GetPosition();
    glm::vec3 box1Extents = pAABB1->GetExtents();

    glm::vec3 box2Pos = pAABBObject2->GetPosition();
    glm::vec3 box2Extents = pAABB2->GetExtents();

    float xOverlap = std::abs(box2Pos.x - box1Pos.x) - (box1Extents.x + box2Extents.x);
    float yOverlap = std::abs(box2Pos.y - box1Pos.y) - (box1Extents.y + box2Extents.y);
    float zOverlap = std::abs(box2Pos.z - box1Pos.z) - (box1Extents.z + box2Extents.z);


    if (xOverlap <= 0 && yOverlap <= 0 && zOverlap <= 0)
    {
        float minOverlap = xOverlap;
        minOverlap = yOverlap < 0 ? std::max(minOverlap, yOverlap) : minOverlap;
        minOverlap = zOverlap < 0 ? std::max(minOverlap, zOverlap) : minOverlap;
        
        glm::vec3 separationNormal(0);

        if (xOverlap == minOverlap) separationNormal.x = 1;
        else if (yOverlap == minOverlap) separationNormal.y = 1;
        else if (zOverlap == minOverlap) separationNormal.z = 1;

        Separate(pAABBObject1, pAABBObject2, -minOverlap, separationNormal);

        return true;
    }

	return false;
}
