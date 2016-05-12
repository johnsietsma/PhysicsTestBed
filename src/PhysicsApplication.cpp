#include "PhysicsApplication.h"

#include "RigidBody.h"
#include "PhysicsObject.h"
#include "Shapes.h"

#include "gl_core_4_4.h"
#include "GLFW/glfw3.h"
#include "Gizmos.h"

#include "glm/ext.hpp"
#include "glm/gtc/quaternion.hpp"

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
    m_camera.setLookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
    m_camera.sensitivity = 3;

    m_pRenderer = std::make_unique<Renderer>();
    m_pPhysicsScene = std::make_unique<PhysicsScene>();


	// Add Ground Plane
	auto pGroundPlane = std::make_shared<PhysicsObject>(
		glm::vec3(0), // Position
		new Plane(glm::vec3(0, 1, 0), 0) // Normal and distance
		);
	m_pPhysicsScene->AddObject(pGroundPlane);

	// Add boxes around the edges
	constexpr int TableSize = 30;
	auto pBox1 = std::make_shared<PhysicsObject>(
		glm::vec3( 0, 0.5f, (TableSize/2)+1 ),		// Position
		new AABB( glm::vec3(TableSize/2, 1, 1) )	// Extents
		);

	auto pBox2 = std::make_shared<PhysicsObject>(
		glm::vec3(0, 0.5f, (-TableSize/2)-1 ),		// Position
		new AABB(glm::vec3(TableSize / 2, 1, 1))	// Extents
		);

	auto pBox3 = std::make_shared<PhysicsObject>(
		glm::vec3((TableSize/2)+1, 0.5f, 0),		// Position
		new AABB(glm::vec3(1, 1, TableSize / 2))	// Extents
		);

	auto pBox4 = std::make_shared<PhysicsObject>(
		glm::vec3((-TableSize/2)-1, 0.5f, 0),		// Position
		new AABB(glm::vec3(1, 1, TableSize / 2))	// Extents
		);

    //m_pPhysicsScene->AddObject(pBox1);
    //m_pPhysicsScene->AddObject(pBox2);
    //m_pPhysicsScene->AddObject(pBox3);
	//m_pPhysicsScene->AddObject(pBox4);


	// Add AABB
	auto pAABB1 = std::make_shared<PhysicsObject>(
		glm::vec3(0, 1, 0),				// Position
		new AABB( glm::vec3(1,1,1) ),   // AABB(extents)
		new RigidBody(1)				// Rigidbody(mass)
		);
	m_pPhysicsScene->AddObject(pAABB1);

    auto pAABB2 = std::make_shared<PhysicsObject>(
        glm::vec3(0, 10, 0),				// Position
        new AABB(glm::vec3(1, 1, 1)),   // AABB(extents)
        new RigidBody(1)				// Rigidbody(mass)
        );
    m_pPhysicsScene->AddObject(pAABB2);

    // Add Sphere
    auto pSphere1 = std::make_shared<PhysicsObject>(
        glm::vec3(0, 10, 0),	 // Position
        new Sphere(1),     // Sphere(radius)
        new RigidBody(1)   // Rigidbody(mass)
        );
    //m_pPhysicsScene->AddObject(pSphere1);



    m_lastFrameTime = (float)glfwGetTime();

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


    return true;
}

void PhysicsApplication::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

    m_pRenderer->RenderAndClear(m_camera.view_proj);

    m_pPhysicsScene->Draw();

    Gizmos::draw(m_camera.proj, m_camera.view);

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void AddWidget(PxShape* shape, PxRigidActor* actor, vec4 geo_color)
{
    PxTransform full_transform = PxShapeExt::getGlobalPose(*shape, *actor);
    vec3 actor_position(full_transform.p.x, full_transform.p.y, full_transform.p.z);
    glm::quat actor_rotation(full_transform.q.w,
        full_transform.q.x,
        full_transform.q.y,
        full_transform.q.z);
    glm::mat4 rot(actor_rotation);

    mat4 rotate_matrix = glm::rotate(10.f, glm::vec3(7, 7, 7));

    PxGeometryType::Enum geo_type = shape->getGeometryType();

    switch (geo_type)
    {
    case (PxGeometryType::eBOX) :
    {
        PxBoxGeometry geo;
        shape->getBoxGeometry(geo);
        vec3 extents(geo.halfExtents.x, geo.halfExtents.y, geo.halfExtents.z);
        Gizmos::addAABBFilled(actor_position, extents, geo_color, &rot);
    } break;
    case (PxGeometryType::eCAPSULE) :
    {
        PxCapsuleGeometry geo;
        shape->getCapsuleGeometry(geo);
        Gizmos::addCapsule(actor_position, geo.halfHeight * 2, geo.radius, 16, 16, geo_color, &rot);
    } break;
    case (PxGeometryType::eSPHERE) :
    {
        PxSphereGeometry geo;
        shape->getSphereGeometry(geo);
        Gizmos::addSphereFilled(actor_position, geo.radius, 16, 16, geo_color, &rot);
    } break;
    case (PxGeometryType::ePLANE) :
    {

    } break;
    }
}

void PhysicsApplication::renderGizmos(PxScene* physics_scene)
{
    PxActorTypeFlags desiredTypes = PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC;
    PxU32 actor_count = physics_scene->getNbActors(desiredTypes);
    PxActor** actor_list = new PxActor*[actor_count];
	physics_scene->getActors(desiredTypes, actor_list, actor_count);
    
    vec4 geo_color(1, 0, 0, 1);
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

    int articulation_count = physics_scene->getNbArticulations();

    for (int a = 0; a < articulation_count; ++a)
    {
        PxArticulation* articulation;
		physics_scene->getArticulations(&articulation, 1, a);

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

