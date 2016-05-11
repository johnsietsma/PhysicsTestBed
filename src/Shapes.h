#pragma once

#include "Gizmos.h"
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

class Shape
{
public:
    virtual void Draw( glm::vec3 position ) = 0;
};


class Sphere : public Shape
{
public:
    Sphere(glm::vec3 center, float radius) :
        m_center(center),
        m_radius(radius)
    {}

    void Draw(glm::vec3 position) override
    {
        Gizmos::addSphere(position + m_center, m_radius, 10, 10, glm::vec4(1, 0, 0, 1));
    }

private:
    glm::vec3 m_center;
    float m_radius;
};


class AABB : public Shape
{
    void Draw(glm::vec3 position) override
    {

    }
};


class Plane : public Shape
{
public:
    Plane(glm::vec3 normal, float distance) :
        m_normal(normal),
        m_distance(distance)
    {}

    void Draw(glm::vec3 position) override
    {
        Gizmos::addAABB(position, glm::vec3(100.f, 0.f, 100.f), glm::vec4(1, 0, 0, 0));
    }

private:
    glm::vec3 m_normal;
    float m_distance;
};