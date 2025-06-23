#ifndef EMISSIVE_H
#define EMISSIVE_H

#include "Ray.hpp"
#include "Point.hpp"
#include "Material.hpp"
#include "triangleArea.hpp"

struct Lambert;

struct Emissive: public Material
{
    RGB color;
    double i;

    Emissive(RGB color, string n = "Emissive"): color(color) {this->name = n; this->i = 1;}
    Emissive(RGB color, double i = 1, string n = "Emissive"): color(color) {this->name = n; this->i = i;}

    ~Emissive() { return; }

    RGB luminance(const Point & p, Ray & o, const Scene & scene) const;
    RGB getDiffuse() const;

    Material * clone() const;
    Material * addMaterial(const Material * m);
};

#endif