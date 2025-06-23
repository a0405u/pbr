#ifndef LIGHT_H
#define LIGHT_H

#include "Vector3.hpp"
#include "Point.hpp"
#include "Ray.hpp"
#include "RGB.hpp"

struct Light
{
    Vector3<double> position;
    RGB color;

    Light() {};
    Light(Vector3<double> pos) {position = pos;}
    Light(Vector3<double> pos, RGB col) {position = pos; color = col;}

    virtual ~Light() { ; }

    virtual Point* trace(Ray & r) const = 0;
    virtual RGB getIntencity(Vector3<double> n) const {return RGB(0, 0, 0);} // Добавить на вход дополнительную точку
    virtual Vector3<double> getPosition() const {return position;} // Либо метод возвращает точку
};

#endif