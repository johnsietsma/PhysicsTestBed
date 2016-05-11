#pragma once

class Sphere;
class Plane;

class Collision
{
    Collision() = delete;
public:



    static bool SphereToSphere(Sphere* pSphere1, Sphere* pSphere2);
    static bool SphereToPlane(Sphere* pSphere, Plane* pPlane);
    static bool PlaneToSphere(Plane* pPlane, Sphere* pSphere);
};
