#ifndef PLANEL_H
#define PLANEL_H

#include "Light.hpp"
#include "triangleArea.hpp"

struct PlaneLight: public Light
{
    double intencity;
    double sizex, sizey;
    Vector3<double> rotation;
    Vector3<double> normal;
    Material * material;

    PlaneLight(double i = 100, Vector3<double> pos = Vector3<double>(), double sx = 1, double sy = 1, Vector3<double> r = Vector3<double>(), RGB col = RGB(1, 1, 1), Material * m = NULL);

    Point* trace(Ray & r) const;
    RGB getIntencity(Vector3<double> n) const;
    Vector3<double> getPosition() const;
};

#endif