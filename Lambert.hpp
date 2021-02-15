#ifndef LAMBERT_H
#define LAMBERT_H

#include "Ray.hpp"
#include "Point.hpp"
#include "Material.hpp"
#include "Scene.hpp"

struct Lambert: public Material
{
    RGB color;

    Lambert(RGB color): color(color) {}
    Lambert(RGB color, string n): color(color) {this->name = n;}

    RGB luminance(Point p, Ray o, const Scene & scene);
};

#endif