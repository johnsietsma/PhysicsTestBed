#pragma once

#include "BasePhysicsScene.h"

#include <glm\vec3.hpp>
#include <memory>

// Fwd decls
namespace physx{
	class PxAllocatorCallback;
	class PxDefaultCpuDispatcher;
	class PxErrorCallback;
	class PxFoundation;
	class PxMaterial;
	class PxPhysics;
	class PxRigidActor;
	class PxScene;
	class PxShape;
	namespace debugger {
		namespace comm {
			class PvdConnection;
		}
	}
	typedef debugger::comm::PvdConnection PxVisualDebuggerConnection;
}


class PhysXScene : public BasePhysicsScene
{
public:
	PhysXScene() : PhysXScene(glm::vec3(0)) {}
	PhysXScene(glm::vec3 offset);
	~PhysXScene();

    void Update(float deltaTime) override;
    void Draw() override;

	void AddPlaneStatic(glm::vec3 normal, float distance) override;
	void AddSphereStatic(glm::vec3 position, float radius) override;
	void AddAABBStatic(glm::vec3 position, glm::vec3 extents) override;

	void AddPlaneDynamic(glm::vec3 normal, float distance, float mass, glm::vec3 velocity) override;
	void AddSphereDynamic(glm::vec3 position, float radius, float mass, glm::vec3 velocity) override;
	void AddAABBDynamic(glm::vec3 position, glm::vec3 extents, float mass, glm::vec3 velocity) override;

private:
	void AddWidget(physx::PxShape* shape, physx::PxRigidActor* actor, glm::vec4 geo_color);
	void SetupVisualDebugger();

	const float DefaultStaticFriction = 0.5f;
	const float DefaultDynamicFriction = 0.5f;
	const float DefaultRestitution = 0.5f;
	const float DefaultDensity = 1;

	physx::PxFoundation* m_pFoundation;
	physx::PxPhysics* m_pPhysics;
	physx::PxScene* m_pScene;
	physx::PxMaterial* m_pDefaultMaterial;
	physx::PxDefaultCpuDispatcher* m_pCPUDispatcher;

	physx::PxVisualDebuggerConnection* m_pConnection;

	std::unique_ptr<physx::PxAllocatorCallback> m_allocatorCallback;
	std::unique_ptr<physx::PxErrorCallback> m_errorCallback;
};
