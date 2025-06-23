#include "ComplexMaterial.hpp"
#include "Lambert.hpp"
#include <iostream>

ComplexMaterial::ComplexMaterial(const Material * m, string n)
{
    material.push_back(m->clone());
    name = n + m->name;
}

RGB ComplexMaterial::luminance(const Point & p, Ray & ray, const Scene & scene) const
{
    unsigned int i = rand() % material.size();
    return material[i]->luminance(p, ray, scene);
}

RGB ComplexMaterial::getDiffuse() const
{
    RGB color = RGB(0, 0, 0);

    for (Material * m : material)
        color = color + m->getDiffuse();

    color = color / material.size();

    return color;
}

Material * ComplexMaterial::clone() const
{
    ComplexMaterial * n = new ComplexMaterial();
    
    for (Material * m : material)
        n->addMaterial(m->clone());

    n->name = name;
    return n;
}

// void ComplexMaterial::addMaterial(const Material * m)
// {
//     for (Material * n : material)
//         if (n->name == m->name and m->name != "")
//             return;
//     material.push_back(m->clone());
//     name += m->name;
// }

Material * ComplexMaterial::addMaterial(const Material * m)
{
    for (Material * n : material)
        if (n->name == m->name and m->name != "")
            return this;
    material.push_back(m->clone());
    name += m->name;
    return this;
}