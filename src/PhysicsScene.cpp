#include "PhysicsScene.h"

#include "PhysicsObject.h"

const glm::vec3 PhysicsScene::DefaultGravity(0.0f, -9.8f, 0.0f);


void PhysicsScene::AddObject(std::shared_ptr<PhysicsObject> pPhysicsObject)
{
    m_pPhysicsObjects.push_back(pPhysicsObject);
};

void PhysicsScene::Update(float deltaTime)
{
    for (auto pPhysicsObject : m_pPhysicsObjects)
    {
        pPhysicsObject->Update(deltaTime, m_gravity);
    }
}

void PhysicsScene::Draw()
{
    for (auto pPhysicsObject : m_pPhysicsObjects)
    {
        pPhysicsObject->Draw();
    }
}
