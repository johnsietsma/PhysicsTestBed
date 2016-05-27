#include "PhysXScene.h"

#include "Gizmos.h"

#include <assert.h>
#include <extensions\PxDefaultAllocator.h>
#include <extensions\PxDefaultErrorCallback.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <PxPhysicsAPI.h>

using namespace physx;

PhysXScene::PhysXScene(glm::vec3 offset) :
	m_allocatorCallback( std::make_unique<PxDefaultAllocator>() ),
	m_errorCallback( std::make_unique<PxDefaultErrorCallback>() ),
	m_pConnection(nullptr)
{
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_allocatorCallback.get(), *m_errorCallback.get());
	assert(m_pFoundation != nullptr);

	m_pPhysics = PxCreateBasePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale());
	assert(m_pPhysics != nullptr);

	m_pCPUDispatcher = PxDefaultCpuDispatcherCreate(1);

	PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, -9.8f, 0);
	sceneDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
	sceneDesc.cpuDispatcher = m_pCPUDispatcher;

	m_pScene = m_pPhysics->createScene(sceneDesc);
	assert(m_pScene != nullptr);

	m_pScene->shiftOrigin(PxVec3(offset.x, offset.y, offset.z));

	m_pDefaultMaterial = m_pPhysics->createMaterial(DefaultStaticFriction, DefaultDynamicFriction, DefaultRestitution);;

	SetupVisualDebugger();
}

PhysXScene::~PhysXScene()
{
	if (m_pConnection != nullptr) {
		m_pConnection->disconnect();
		m_pConnection->release();
	}

	m_pScene->release();
	m_pCPUDispatcher->release();
	m_pPhysics->release();
	m_pFoundation->release();

}


void PhysXScene::AddPlaneStatic(glm::vec3 normal, float distance) 
{
	PxTransform transform = PxTransformFromPlaneEquation(PxPlane(normal.x, normal.y, normal.z, distance));
	PxRigidStatic* pPlane = PxCreateStatic(*m_pPhysics, transform, PxPlaneGeometry(), *m_pDefaultMaterial);
	m_pScene->addActor(*pPlane);
}


void PhysXScene::AddSphereStatic(glm::vec3 position, float radius) 
{
	PxTransform transform(position.x, position.y, position.z);
	PxSphereGeometry sphereGeo(radius);
	PxRigidStatic* pSphere = PxCreateStatic(*m_pPhysics, transform, sphereGeo, *m_pDefaultMaterial);
	m_pScene->addActor(*pSphere);
}

void PhysXScene::AddAABBStatic(glm::vec3 position, glm::vec3 extents) 
{
	PxTransform transform(position.x, position.y, position.z);
	PxBoxGeometry boxGeo(extents.x, extents.y, extents.z);
	PxRigidStatic* pBox = PxCreateStatic(*m_pPhysics, transform, boxGeo, *m_pDefaultMaterial);
	m_pScene->addActor(*pBox);
}


void PhysXScene::AddPlaneDynamic(glm::vec3 normal, float distance, float mass, glm::vec3 velocity) 
{
	PxTransform transform = PxTransformFromPlaneEquation(PxPlane(normal.x, normal.y, normal.z, distance));
	PxRigidDynamic* pPlane = PxCreateDynamic(*m_pPhysics, transform, PxPlaneGeometry(), *m_pDefaultMaterial, DefaultDensity);
	pPlane->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
	m_pScene->addActor(*pPlane);
}


void PhysXScene::AddSphereDynamic(glm::vec3 position, float radius, float mass, glm::vec3 velocity) 
{
	PxTransform transform(position.x, position.y, position.z);
	PxSphereGeometry sphereGeo(radius);
	PxRigidDynamic* pSphere = PxCreateDynamic(*m_pPhysics, transform, sphereGeo, *m_pDefaultMaterial, DefaultDensity);
	pSphere->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
	m_pScene->addActor(*pSphere);
}

void PhysXScene::AddAABBDynamic(glm::vec3 position, glm::vec3 extents, float mass, glm::vec3 velocity) 
{
	PxTransform transform(position.x, position.y, position.z);
	PxBoxGeometry boxGeo(extents.x, extents.y, extents.z);
	PxRigidDynamic* pBox = PxCreateDynamic(*m_pPhysics, transform, boxGeo, *m_pDefaultMaterial, DefaultDensity);
	pBox->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
	m_pScene->addActor(*pBox);
}



void PhysXScene::Update(float deltaTime)
{
	m_pScene->simulate(deltaTime);
	while (m_pScene->fetchResults() == false) {
		// TODO
	}
}

void PhysXScene::Draw()
{
	PxActorTypeFlags desiredTypes = PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC;
	PxU32 actor_count = m_pScene->getNbActors(desiredTypes);
	PxActor** actor_list = new PxActor*[actor_count];
	m_pScene->getActors(desiredTypes, actor_list, actor_count);

	glm::vec4 geo_color(1, 0, 0, 1);
	for (int actor_index = 0;
		actor_index < (int)actor_count;
		++actor_index)
	{
		PxActor* curr_actor = actor_list[actor_index];
		if (curr_actor->isRigidActor())
		{
			PxRigidActor* rigid_actor = (PxRigidActor*)curr_actor;
			PxU32 shape_count = rigid_actor->getNbShapes();
			PxShape** shapes = new PxShape*[shape_count];
			rigid_actor->getShapes(shapes, shape_count);

			for (int shape_index = 0;
				shape_index < (int)shape_count;
				++shape_index)
			{
				PxShape* curr_shape = shapes[shape_index];
				AddWidget(curr_shape, rigid_actor, geo_color);
			}

			delete[]shapes;
		}
	}

	delete[] actor_list;

	int articulation_count = m_pScene->getNbArticulations();

	for (int a = 0; a < articulation_count; ++a)
	{
		PxArticulation* articulation;
		m_pScene->getArticulations(&articulation, 1, a);

		int link_count = articulation->getNbLinks();

		PxArticulationLink** links = new PxArticulationLink*[link_count];
		articulation->getLinks(links, link_count);

		for (int l = 0; l < link_count; ++l)
		{
			PxArticulationLink* link = links[l];
			int shape_count = link->getNbShapes();

			for (int s = 0; s < shape_count; ++s)
			{
				PxShape* shape;
				link->getShapes(&shape, 1, s);
				AddWidget(shape, link, geo_color);
			}
		}
		delete[] links;
	}

}

void PhysXScene::AddWidget(PxShape* shape, PxRigidActor* actor, glm::vec4 geo_color)
{
	PxTransform full_transform = PxShapeExt::getGlobalPose(*shape, *actor);
	glm::vec3 actor_position(full_transform.p.x, full_transform.p.y, full_transform.p.z);
	glm::quat actor_rotation(full_transform.q.w,
		full_transform.q.x,
		full_transform.q.y,
		full_transform.q.z);
	glm::mat4 rot(actor_rotation);

	glm::mat4 rotate_matrix = glm::rotate(10.f, glm::vec3(7, 7, 7));

	PxGeometryType::Enum geo_type = shape->getGeometryType();

	switch (geo_type)
	{
	case (PxGeometryType::eBOX):
	{
		PxBoxGeometry geo;
		shape->getBoxGeometry(geo);
		glm::vec3 extents(geo.halfExtents.x, geo.halfExtents.y, geo.halfExtents.z);
		Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
	} break;
	case (PxGeometryType::eCAPSULE):
	{
		PxCapsuleGeometry geo;
		shape->getCapsuleGeometry(geo);
		Gizmos::addCapsule(actor_position, geo.halfHeight * 2, geo.radius, 16, 16, geo_color, &rot);
	} break;
	case (PxGeometryType::eSPHERE):
	{
		PxSphereGeometry geo;
		shape->getSphereGeometry(geo);
		Gizmos::addSphereFilled(actor_position, geo.radius, 16, 16, geo_color, &rot);
	} break;
	case (PxGeometryType::ePLANE):
	{

	} break;
	}
}



void PhysXScene::SetupVisualDebugger()
{
	if (m_pPhysics == nullptr) return;

	const char* pHostIp = "127.0.0.1";
	const int port = 5425;
	const int timeout = 100;

	auto connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();
	m_pConnection = PxVisualDebuggerExt::createConnection(m_pPhysics->getPvdConnectionManager(), pHostIp, port, timeout, connectionFlags);
}
