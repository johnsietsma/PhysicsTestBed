#pragma once

#include "Gizmos.h"
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

class Shape
{
public:
	static constexpr int GetShapeCount() { return static_cast<int>(ID::Count); }
	int GetID() const { return static_cast<int>(m_id); }
    virtual void Draw( glm::vec3 position ) const = 0;

protected:
	enum class ID { Plane, Sphere, AABB, Count };

	Shape(ID id) : m_id(id) {}

private:
	ID m_id;
};


class Sphere : public Shape
{
public:
    Sphere(float radius) :
		Shape(ID::Sphere),
        m_radius(radius)
    {}

	float GetRadius() const { return m_radius; }

    void Draw(glm::vec3 position) const override
    {
        Gizmos::addSphereFilled(position, m_radius, 10, 10, glm::vec4(0.5f, 0, 0, 1));
    }

private:
    float m_radius;
};


class AABB : public Shape
{
public:
	AABB( glm::vec3 extents ) : Shape(ID::AABB), m_extents(extents) {}

	glm::vec3 GetMin() const { return -m_extents; }
	glm::vec3 GetMax() const { return m_extents; }

    void Draw(glm::vec3 position) const override
    {
		Gizmos::addAABBFilled(position, m_extents*2.f, glm::vec4(0, 0.5f, 0, 1));
    }

private:
	glm::vec3 m_extents;
};


class Plane : public Shape
{
public:
    Plane(glm::vec3 normal, float distance) :
		Shape(ID::Plane),
        m_normal(normal),
        m_distance(distance)
    {}

	glm::vec3 GetNormal() const { return m_normal; }
	float GetDistance() const { return m_distance;  }

    void Draw(glm::vec3 position) const override
    {
        Gizmos::addAABBFilled(position, glm::vec3(100.f, 0.f, 100.f), glm::vec4(0.25f, 0.25f, 0.25f, 1));
    }

private:
    glm::vec3 m_normal;
    float m_distance; // Distance from the origin
};