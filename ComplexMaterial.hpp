#ifndef COMPLEXMATERIAL_H
#define COMPLEXMATERIAL_H

#include "Ray.hpp"
#include "Point.hpp"
#include "Material.hpp"
#include "Emissive.hpp"
#include "Lambert.hpp"
#include "Scene.hpp"


struct ComplexMaterial: public Material
{
    vector<Material *> material;

    ComplexMaterial(string n = "Complex") { this->name = n; }
    ComplexMaterial(const Material * m, string n = "Complex");

    ~ComplexMaterial() { for (Material * m : material) delete m; }

    RGB luminance(const Point & p, Ray & o, const Scene & scene) const;
    RGB getDiffuse() const;

    //void addMaterial(const Material * m);

    Material * clone() const;
    Material * addMaterial(const Material * m);
};

#endif