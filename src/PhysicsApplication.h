#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "Render.h"
#include "PhysicsScene.h"

#include <memory>
#include <random>
#include <PxScene.h>


class PhysicsApplication : public Application
{
public:
	PhysicsApplication() : m_distribution(-20, 20) {}

	virtual bool startup();
	virtual void shutdown();
    virtual bool update();
    virtual void draw();

    void renderGizmos(physx::PxScene* physics_scene);

    std::unique_ptr<PhysicsScene> m_pPhysicsScene;
    std::unique_ptr<Renderer> m_pRenderer;
    FlyCamera m_camera;
    float m_lastFrameTime;
	float m_emitTimer;

	std::default_random_engine m_generator;
	std::uniform_real_distribution<float> m_distribution;
};



#endif //CAM_PROJ_H_
