#ifndef SOUND_PROGRAMMING_H_
#define SOUND_PROGRAMMING_H_

#include "Application.h"
#include "Camera.h"
#include "Render.h"
#include "PhysicsScene.h"

#include <memory>

#include <PxScene.h>


class PhysicsApplication : public Application
{
public:
	virtual bool startup();
	virtual void shutdown();
    virtual bool update();
    virtual void draw();

    void renderGizmos(physx::PxScene* physics_scene);

    std::unique_ptr<PhysicsScene> m_pPhysicsScene;
    std::unique_ptr<Renderer> m_pRenderer;
    FlyCamera m_camera;
    float m_leftFrameTime;
};



#endif //CAM_PROJ_H_
