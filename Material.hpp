#ifndef MATERIAL_H
#define MATERIAL_H

#include "RGB.hpp"

struct Point;
struct Ray;
struct Scene;
struct Lambert;
struct Emissive;

struct Material
{
    string name;

    virtual ~Material() 
    { 
        return; 
    }

    virtual RGB luminance(const Point & p, Ray & o, const Scene & scene) const = 0;
    virtual RGB getDiffuse() const = 0;
    virtual Material * clone() const = 0;
    virtual Material * addMaterial(const Material * m) = 0;

    // virtual bool operator == (const Material & m) const = 0;
};

#endif