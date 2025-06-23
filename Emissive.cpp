#include "Emissive.hpp"
#include "Lambert.hpp"
#include "ComplexMaterial.hpp"
#include "Constants.hpp"
#include <iostream>

RGB Emissive::luminance(const Point & point, Ray & ray, const Scene & scene) const
{
    if (ray.count >= RAY_REREFRACTIONS - 1)
        return color * i / cosVector(point.normal, -ray.direction); // / LIGHT_MULTIPLIER; // / ray.length; // Возвращение яркости
    ray.count = 0;
    return RGB();
}

RGB Emissive::getDiffuse() const
{
    return color;
}

Material * Emissive::clone() const
{
    return new Emissive( this->color, this->i, this->name);
}

Material * Emissive::addMaterial(const Material * m)
{
    if (name == m->name and m->name != "")
        return this;
    
    Material * newMaterial = new ComplexMaterial(this);
    newMaterial->addMaterial(m);
    return newMaterial;
}