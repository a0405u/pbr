#ifndef LAMBERT_H
#define LAMBERT_H

#include "Ray.hpp"
#include "Point.hpp"
#include "Material.hpp"
#include "Scene.hpp"

struct Emissive;

struct Lambert: public Material
{
    RGB color;

    Lambert(RGB color, string n = "Lambert"): color(color) {this->name = n;}

    ~Lambert() { return; }

    RGB luminance(const Point & p, Ray & o, const Scene & scene) const;
    RGB getDiffuse() const;

    Material * clone() const;
    Material * addMaterial(const Material * m);
};

#endif