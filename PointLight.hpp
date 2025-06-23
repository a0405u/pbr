#ifndef POINTL_H
#define POINTL_H

#include "Light.hpp"

struct PointLight: public Light
{
    double flux;

    PointLight();
    PointLight(double f);
    PointLight(double f, Vector3<double> pos);
    PointLight(double f, Vector3<double> pos, RGB col);

    Point* trace(Ray & r) const { return NULL; }
    RGB getIntencity(Vector3<double> n) const;
    
};

#endif