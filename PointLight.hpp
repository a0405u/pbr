#ifndef POINTL_H
#define POINTL_H

#include "Light.hpp"

struct PointLight: public Light
{
    double flux;

    PointLight();
    PointLight(double f);
    PointLight(double f, Vector3 pos);
    PointLight(double f, Vector3 pos, RGB col);

    RGB getIntencity(Vector3 n) const;
    
};

#endif