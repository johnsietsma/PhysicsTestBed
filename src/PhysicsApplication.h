#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "BasePhysicsScene.h"
#include "Camera.h"
#include "Render.h"

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

	std::unique_ptr<BasePhysicsScene> m_pPhysicsScene;
	std::unique_ptr<BasePhysicsScene> m_pPhysXScene;
    std::unique_ptr<Renderer> m_pRenderer;

    FlyCamera m_camera;
    float m_lastFrameTime;
	float m_emitTimer;

	std::default_random_engine m_generator;
	std::uniform_real_distribution<float> m_distribution;

private:
	void CreateBoundary(BasePhysicsScene* pPhysicsScene, float tableSize, float borderHeight);
	void CreateSpheres(BasePhysicsScene* pPhysicsScene, int sphereCount, float spacing);
	void CreateAABBs(BasePhysicsScene* pPhysicsScene, int aabbCount, float spacing);
};


#endif //CAM_PROJ_H_
