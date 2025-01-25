#pragma once
#include "Common.h"

enum class PlaneRelation
{
    PR_BACK,
    PR_FRONT,
    PR_ON,
};

class Plane
{
public:
    Plane() = default;
    Plane(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);

    float distanceToPoint(const glm::vec3& v)const;
    PlaneRelation relation(const glm::vec3& v, float epsilon = 0.0001f)const;

private:
    glm::vec3 normal = { 0 , 0 , 1 };
    float d = 0;
};


inline Plane::Plane(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
   // normal = (v2 - v1).Cross(v3 - v1);
    normal = glm::cross(v2 - v1, v3 - v1);
    normal = glm::normalize(normal);
    //d = -(normal * v1);
    d = -glm::dot(normal, v1);
}

inline float Plane::distanceToPoint(const glm::vec3& v) const
{
   // normal * v + d;
    return glm::dot(normal, v) + d;
}

inline PlaneRelation Plane::relation(const glm::vec3& v, float epsilon) const
{
    float dist = distanceToPoint(v);

    if (dist < -epsilon)
        return PlaneRelation::PR_BACK;
    else if (dist > epsilon)
        return PlaneRelation::PR_FRONT;
    else
        return PlaneRelation::PR_ON;
}