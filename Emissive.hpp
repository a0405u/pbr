#ifndef EMISSIVE_H
#define EMISSIVE_H

#include "Ray.hpp"
#include "Point.hpp"
#include "Material.hpp"
#include "triangleArea.hpp"

struct Emissive: public Material
{
    RGB color;
    double i;

    Emissive(RGB color): color(color) {i = 100;}
    Emissive(RGB color, string n): color(color) {this->name = n; i = 100;}
    Emissive(RGB color, double i, string n): color(color) {this->name = n; this->i = i;}

    RGB luminance(Point p, Ray o, const Scene & scene);
};

#endif