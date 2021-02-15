#ifndef PLANEL_H
#define PLANEL_H

#include "Light.hpp"
#include "triangleArea.hpp"

struct PlaneLight: public Light
{
    double intencity;
    double sizex, sizey;
    Vector3 rotation;
    Vector3 normal;

    PlaneLight(double i = 100, Vector3 pos = Vector3(), double sx = 1, double sy = 1, Vector3 r = Vector3(), RGB col = RGB(1, 1, 1));

    RGB getIntencity(Vector3 n) const;
    Vector3 getPosition() const;
};

#endif