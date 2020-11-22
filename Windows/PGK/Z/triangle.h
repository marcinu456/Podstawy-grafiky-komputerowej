#ifndef TRIANGLEPOINTCOLLISION_H
#define TRIANGLEPOINTCOLLISION_H
#include <iostream>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class Triangle
{
public:
    struct Edge3
    {
        const glm::vec3 A;
        const glm::vec3 B;
        const glm::vec3 Delta;
        const float sqrtLength;

        Edge3(glm::vec3 a, glm::vec3 b) : A(a), B(b), Delta(b - a), sqrtLength(glm::sqrt(glm::length(Delta))){}
        glm::vec3 pointAt(float t) const
        {
            return A + Delta * t;
        }
        float lengthSquared() const
        {
            return sqrtLength;
        }
        float project(glm::vec3 p) const
        {
            return glm::dot(p - A, Delta) / sqrtLength;
        }

    };

    struct Plane
    {
        const glm::vec3 Point;
        const glm::vec3 Direction;

        Plane(glm::vec3 point, glm::vec3 direction) : Point(point), Direction(direction) {}
        bool isAbove(glm::vec3 q) const
        {
            return glm::dot(Direction, q - Point) > 0;
        }

        glm::vec3 project(glm::vec3 p) const
        {
            glm::vec3 normalizedDirecion = Direction/glm::length(Direction);
            return p - (glm::dot(p, normalizedDirecion) - glm::dot(Point, normalizedDirecion)) *normalizedDirecion;
        }
    };
    const glm::vec3 A;
    const glm::vec3 B;
    const glm::vec3 C;

    const Edge3 edgeAB;
    const Edge3 edgeBC;
    const Edge3 edgeCA;


    const glm::vec3 triangleNormal;

    const Plane trianglePlain;
    const Plane planeAB;
    const Plane planeBC;
    const Plane planeCA;
    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) : A(a), B(b), C(c), edgeAB(a, b), edgeBC(b, c), edgeCA(c, a), triangleNormal(glm::cross(a - b, a - c)),
        trianglePlain(a, triangleNormal), planeAB(a, glm::cross(triangleNormal, edgeAB.Delta)), planeBC(b, glm::cross(triangleNormal, edgeBC.Delta)), planeCA(c, glm::cross(triangleNormal, edgeCA.Delta)) {}

    glm::vec3 nearestPoint(glm::vec3 p)
    {
        float uAB = edgeAB.project(p);
        float uCA = edgeCA.project(p);

        if(uCA > 1 && uAB < 0)
        {
            return A;
        }

        float uBC = edgeBC.project(p);

        if(uAB > 1 && uBC < 0)
        {
            return B;
        }

        if(uBC > 1 && uCA < 0)
        {
            return C;
        }

        if(uAB > 0 && uAB < 1 && !planeAB.isAbove(p))
        {
            return edgeAB.pointAt(uAB);
        }

        if(uBC > 0 && uBC < 1 && !planeBC.isAbove(p))
        {
            return edgeBC.pointAt(uBC);
        }

        if(uCA > 0 && uCA < 1 && !planeCA.isAbove(p))
        {
            return edgeCA.pointAt(uCA);
        }

        return trianglePlain.project(p);

    }
};

#endif // TRIANGLEPOINTCOLLISION_H
