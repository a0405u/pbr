#ifndef LIGHT_H
#define LIGHT_H

#include "Vector3.hpp"
#include "RGB.hpp"

struct Light
{
    Vector3 position;
    RGB color;

    Light() {};
    Light(Vector3 pos) {position = pos;}
    Light(Vector3 pos, RGB col) {position = pos; color = col;}

    virtual RGB getIntencity(Vector3 n) const {return RGB(0, 0, 0);}
    virtual Vector3 getPosition() const {return position;}
};

#endif