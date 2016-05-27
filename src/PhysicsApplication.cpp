#include "PhysicsApplication.h"

#include "RigidBody.h"
#include "PhysicsScene.h"
#include "PhysXScene.h"
#include "PhysicsObject.h"
#include "Shapes.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"

#include "glm/ext.hpp"
#include "glm/gtc/quaternion.hpp"

#include <functional>
#include <PxPhysicsAPI.h>

using namespace physx;

#define Assert(val) if (val){}else{ *((char*)0) = 0;}
#define ArrayCount(val) (sizeof(val)/sizeof(val[0]))

bool PhysicsApplication::startup()
{
    if (Application::startup() == false)
    {
        return false;
    }
	
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    Gizmos::create();

    m_camera = FlyCamera(1280.0f / 720.0f, 10.0f);
    m_camera.setLookAt(vec3(50, 50, 50), vec3(0), vec3(0, 1, 0));
    m_camera.sensitivity = 3;

    m_pRenderer = std::make_unique<Renderer>();
	m_pPhysicsScene = std::make_unique<PhysicsScene>( glm::vec3(-70,0,0)) ;
	m_pPhysXScene = std::make_unique<PhysXScene>( glm::vec3(70, 0, 0) );


	const float TableSize = 60;
	const float BorderHeight = 15;

	// Add Ground Plane
	m_pPhysicsScene->AddPlaneStatic(glm::vec3(0, 1, 0), 0);
	CreateBoundary(m_pPhysicsScene.get(), TableSize, BorderHeight);
	CreateSpheres(m_pPhysicsScene.get(), 20, 2);
	CreateAABBs(m_pPhysicsScene.get(), 20, 2.1f);

	m_pPhysXScene->AddPlaneStatic(glm::vec3(0, 1, 0), 0);
	CreateBoundary(m_pPhysXScene.get(), TableSize, BorderHeight);
	CreateSpheres(m_pPhysXScene.get(), 20, 2);
	CreateAABBs(m_pPhysXScene.get(), 20, 2.1f);

    m_lastFrameTime = (float)glfwGetTime();
	m_emitTimer = 0;

    return true;
}

void PhysicsApplication::shutdown()
{
    Gizmos::destroy();
    Application::shutdown();
}

bool PhysicsApplication::update()
{
    if (Application::update() == false)
    {
        return false;
    }

    Gizmos::clear();

    float currentTime = (float)glfwGetTime();
    float deltaTime = currentTime - m_lastFrameTime;
    m_lastFrameTime = currentTime;

	m_emitTimer += deltaTime;

	if (m_emitTimer > 0.5f) {
		float velX = m_distribution(m_generator);
		float velY = m_distribution(m_generator);
		float velZ = m_distribution(m_generator);

		m_pPhysicsScene->AddSphereDynamic(
			glm::vec3(0,3,0),			// Position
			1,							// Radius
			1, glm::vec3(velX, velY, velZ)	// Mass, vel
		);

		m_pPhysXScene->AddSphereDynamic(
			glm::vec3(0, 3, 0),			// Position
			1,							// Radius
			1, glm::vec3(velX, velY, velZ)	// Mass, vel
		);

		m_emitTimer = 0;
	}

    vec4 white(1);
    vec4 black(0, 0, 0, 1);

    for (int i = 0; i <= 20; ++i)
    {
        Gizmos::addLine(vec3(-10 + i, -0.01, -10), vec3(-10 + i, -0.01, 10),
            i == 10 ? white : black);
        Gizmos::addLine(vec3(-10, -0.01, -10 + i), vec3(10, -0.01, -10 + i),
            i == 10 ? white : black);
    }

    m_camera.update(deltaTime);

	m_pPhysicsScene->Update(deltaTime);
	m_pPhysXScene->Update(deltaTime);


    return true;
}

void PhysicsApplication::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

    m_pRenderer->RenderAndClear(m_camera.view_proj);

	m_pPhysicsScene->Draw();
	m_pPhysXScene->Draw();

    Gizmos::draw(m_camera.proj, m_camera.view);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void PhysicsApplication::CreateBoundary(BasePhysicsScene* pPhysicsScene, float tableSize, float borderHeight )
{
	pPhysicsScene->AddAABBStatic(
		glm::vec3(0, 0.5f, (tableSize / 2) + 1),	// Position
		glm::vec3(tableSize / 2, borderHeight, 1)	// Extents
	);

	pPhysicsScene->AddAABBStatic(
		glm::vec3(0, 0.5f, (-tableSize / 2) - 1),		// Position
		glm::vec3(tableSize / 2, borderHeight, 1)	// Extents
	);

	pPhysicsScene->AddAABBStatic(
		glm::vec3((tableSize / 2) + 1, 0.5f, 0),		// Position
		glm::vec3(1, borderHeight, tableSize / 2)	// Extents
	);

	pPhysicsScene->AddAABBStatic(
		glm::vec3((-tableSize / 2) - 1, 0.5f, 0),		// Position
		glm::vec3(1, borderHeight, tableSize / 2)	// Extents
	);


}

void PhysicsApplication::CreateSpheres(BasePhysicsScene* pPhysicsScene, int sphereCount, float spacing)
{
	auto randVel = std::bind(m_distribution, m_generator);
	for (int i = 0; i < sphereCount; i++) {
		pPhysicsScene->AddSphereDynamic(
			glm::vec3(-20 + i*spacing, 2 + i, -20 + i*spacing),		// Position
			1,													// Radius
			1, glm::vec3(randVel(), 0, randVel())					// mass, vel
		);
	}

}

void PhysicsApplication::CreateAABBs(BasePhysicsScene* pPhysicsScene, int aabbCount, float spacing)
{
	auto randVel = std::bind(m_distribution, m_generator);
	for (int i = 0; i < aabbCount; i++) {
		pPhysicsScene->AddAABBDynamic(
			glm::vec3(-20 + i*spacing, 6 + i, -20 + i*spacing),	// Position
			glm::vec3(1, 1, 1),									// Extents
			1, glm::vec3(randVel(), 0, randVel())					// mass,vel
		);
	}

}
