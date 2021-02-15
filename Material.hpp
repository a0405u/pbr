#ifndef MATERIAL_H
#define MATERIAL_H

#include "RGB.hpp"

struct Point;
struct Ray;
struct Scene;

struct Material
{
    string name;
    virtual RGB luminance(Point p, Ray o, const Scene & scene) = 0;
};

#endif