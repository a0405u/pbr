#ifndef VOXEL_H
#define VOXEL_H

#include <iostream>
#include "Vector3.hpp"
#include "Material.hpp"
#include "RGB.hpp"

using namespace std;

struct Voxel
{
    Vector3<double> normal;

    Material * material; // Индекс вместо указателя для уменьшения занимаемой памяти

    Voxel(const Voxel & v) { normal = v.normal; material = v.material; }
    Voxel(Material * m = nullptr): material(m) { normal = Vector3<double>(0, 0, 0); }
    Voxel(const Vector3<double> n, Material * m = nullptr): normal(n), material(m) { ; }
    ~Voxel() { return; }

    Material * newMaterial(Material * m) { if (material and material != m) delete material; return material = m; }
    Material * addMaterial(Material * m)
    {
        if (not material)
            material = m;
        else
            material = material->addMaterial(m);
        return material;
    };
};

#endif