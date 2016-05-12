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

	// Separation relative to the mass of the objects
	glm::vec3 separationVector = normal * overlap;
	pObject1->Translate(-separationVector * massRatio2);
	pObject2->Translate(separationVector * massRatio1);
}

void Response( PhysicsObject* pObject1, PhysicsObject* pObject2, float overlap, glm::vec3 normal )
{
	Separate(pObject1, pObject2, overlap, normal);

	const float coefficientOfRestitution = 0.9f;

	// Calculate the momentum along the collision normal
	float impulse1 = -(1 + coefficientOfRestitution) * glm::dot(pObject1->GetMomentum(), normal);
	float impulse2 = -(1 + coefficientOfRestitution) * glm::dot(pObject2->GetMomentum(), normal);

	// Apply the change in momentum
    pObject1->AddMomentum(impulse1 * normal);
    pObject2->AddMomentum(impulse2 * normal);
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
	if (overlap < 0)
	{
		Response(pPlaneObject, pSphereObject, -overlap, planeNormal);
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

	float overlap = std::min(minPointDistanceAlongPlaneNormal, maxPointDistanceAlongPlaneNormal);

	if(overlap < 0 ) {
		Response(pPlaneObject, pAABBObject, -overlap, pPlane->GetNormal());
		return true;
	}

	return false;
}

bool Collision::PlaneToPlane(PhysicsObject* pPlaneObject1, PhysicsObject* pPlaneObject2)
{
    // Not going to implement this right now.
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
	const auto pSphere2 = pSphereObject2->GetShape<Sphere>();

	glm::vec3 directionVector = pSphereObject2->GetPosition() - pSphereObject1->GetPosition();
	float centerDistance = glm::length(directionVector);
	float radiusDistance = pSphere1->GetRadius() + pSphere2->GetRadius();

	float overlap = centerDistance - radiusDistance;
	if (overlap < 0) {
		Response(pSphereObject1, pSphereObject2, -overlap, glm::normalize(directionVector) );
		return true;
	}

	return false;
}

bool Collision::SphereToAABB(PhysicsObject* pSphereObject, PhysicsObject* pAABBObject)
{
    const auto pSphere = pSphereObject->GetShape<Sphere>();
    const auto pAABB = pAABBObject->GetShape<AABB>();

    glm::vec3 minPos = -pAABB->GetExtents();
    glm::vec3 maxPos = pAABB->GetExtents();

    glm::vec3 distance = pSphereObject->GetPosition() - pAABBObject->GetPosition();
	glm::vec3 clampedPoint = distance;

    if (distance.x < minPos.x)
        clampedPoint.x = minPos.x;
    else if (distance.x > maxPos.x)
        clampedPoint.x = maxPos.x;

    if (distance.y < minPos.y)
        clampedPoint.y = minPos.y;
    else if (distance.y > maxPos.y)
        clampedPoint.y = maxPos.y;

    if (distance.z < minPos.z)
        clampedPoint.z = minPos.z;
    else if (distance.z > maxPos.z)
        clampedPoint.z = maxPos.z;

    glm::vec3 clampedDistance = distance - clampedPoint;

    float overlap = glm::length(clampedDistance) - pSphere->GetRadius();
    if (overlap < 0)
    {
        Response(pAABBObject, pSphereObject, -overlap, glm::normalize(clampedDistance));
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

	glm::vec3 boxDelta = box2Pos - box1Pos;
	glm::vec3 boxExtentsCombined = box1Extents + box2Extents;

    float xOverlap = std::abs(boxDelta.x) - boxExtentsCombined.x;
    float yOverlap = std::abs(boxDelta.y) - boxExtentsCombined.y;
    float zOverlap = std::abs(boxDelta.z) - boxExtentsCombined.z;


    if (xOverlap <= 0 && yOverlap <= 0 && zOverlap <= 0)
    {
        float minOverlap = xOverlap;
        minOverlap = yOverlap < 0 ? std::max(minOverlap, yOverlap) : minOverlap;
        minOverlap = zOverlap < 0 ? std::max(minOverlap, zOverlap) : minOverlap;
        
        glm::vec3 separationNormal(0);

        if (xOverlap == minOverlap) separationNormal.x = std::signbit(boxDelta.x) ? -1.f : 1.f;
        else if (yOverlap == minOverlap) separationNormal.y = std::signbit(boxDelta.y) ? -1.f : 1.f;
        else if (zOverlap == minOverlap) separationNormal.z = std::signbit(boxDelta.z) ? -1.f : 1.f;

        Response(pAABBObject1, pAABBObject2, -minOverlap, separationNormal);

        return true;
    }

	return false;
}
