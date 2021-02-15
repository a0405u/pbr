#ifndef POINT_H
#define POINT_H

#include "Vector3.hpp"
#include "Material.hpp"

struct Scene;
struct Light;

struct Point
{
    Vector3 position;
    Vector3 normal;
    Material * material;
    RGB illuminance;

    Point(Vector3 p, Vector3 n, Material * m): position(p), normal(n), material(m) {}

    RGB calcIlluminance(const Scene & scene) const; // Рассчитать освещенность в точке от всей сцены
    RGB calcIlluminance(const Light & light, const Scene & scene) const; // Рассчитать освещенность в точке от одного источника
};

#endif