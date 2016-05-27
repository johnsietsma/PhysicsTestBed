#include "PhysicsScene.h"

#include "Collision.h"
#include "PhysicsObject.h"
#include"RigidBody.h"


void PhysicsScene::AddPlaneStatic(glm::vec3 normal, float distance)
{
	AddPlane(normal, distance);
}

void PhysicsScene::AddSphereStatic(glm::vec3 position, float radius)
{
	AddSphere(position, radius);
}

void PhysicsScene::AddAABBStatic(glm::vec3 position, glm::vec3 extents)
{
	AddAABB(position, extents);
}


void PhysicsScene::AddPlaneDynamic(glm::vec3 normal, float distance, float mass, glm::vec3 velocity)
{
	AddPlane(normal, distance, new RigidBody(mass, velocity));
}

void PhysicsScene::AddSphereDynamic(glm::vec3 position, float radius, float mass, glm::vec3 velocity)
{
	AddSphere(position, radius, new RigidBody(mass, velocity));
}

void PhysicsScene::AddAABBDynamic(glm::vec3 position, glm::vec3 extents, float mass, glm::vec3 velocity)
{
	AddAABB(position, extents, new RigidBody(mass, velocity));
}

void PhysicsScene::AddPlane(glm::vec3 normal, float distance, RigidBody* pRigidBody)
{
	auto pPlane = std::make_shared<PhysicsObject>(
		glm::vec3(0),				// Position, not used for plane
		new Plane(normal, distance), // Normal and distance
		pRigidBody
		);
	AddObject(pPlane);
}

void PhysicsScene::AddSphere(glm::vec3 position, float radius, RigidBody* pRigidBody)
{
	auto pSphere = std::make_shared<PhysicsObject>(position, new Sphere(radius), pRigidBody);
	AddObject(pSphere);
}

void PhysicsScene::AddAABB(glm::vec3 position, glm::vec3 extents, RigidBody* pRigidBody)
{
	auto pBox = std::make_shared<PhysicsObject>(position, new AABB(extents), pRigidBody);
	AddObject(pBox);
}

void PhysicsScene::AddObject(std::shared_ptr<PhysicsObject> pPhysicsObject)
{
	pPhysicsObject->Translate(m_offset);
    m_pPhysicsObjects.push_back(pPhysicsObject);
};

void PhysicsScene::Update(float deltaTime)
{
	const float DampingCoeffecient = 0.2f;

	for (auto& pPhysicsObject : m_pPhysicsObjects)
	{
		pPhysicsObject->AddForce(DampingCoeffecient * -pPhysicsObject->GetVelocity() * deltaTime);
	}

    for (auto& pPhysicsObject : m_pPhysicsObjects)
    {
        pPhysicsObject->Update(deltaTime, m_gravity);
    }

	CheckCollisions();
}

void PhysicsScene::Draw()
{
    for (auto& pPhysicsObject : m_pPhysicsObjects)
    {
        pPhysicsObject->Draw();
    }
}

void PhysicsScene::CheckCollisions()
{
    for (auto it1 = std::begin(m_pPhysicsObjects); it1 != std::end(m_pPhysicsObjects); it1++)
    {
        for (auto it2 = std::next(it1); it2 != std::end(m_pPhysicsObjects); it2++)
        {
			Collision::Detect(it1->get(), it2->get());
        }
    }

}
