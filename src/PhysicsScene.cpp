#include "PhysicsScene.h"

#include "Collision.h"
#include "PhysicsObject.h"

const glm::vec3 PhysicsScene::DefaultGravity(0.0f, -9.8f, 0.0f);


void PhysicsScene::AddObject(std::shared_ptr<PhysicsObject> pPhysicsObject)
{
    m_pPhysicsObjects.push_back(pPhysicsObject);
};

void PhysicsScene::Update(float deltaTime)
{
	for (auto& pPhysicsObject : m_pPhysicsObjects)
	{
		//pPhysicsObject->AddForce(DampeningCoeffecient * -pPhysicsObject->GetVelocity());
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
