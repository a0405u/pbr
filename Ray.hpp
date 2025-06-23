#ifndef RAY_H
#define RAY_H

#include "Vector3.hpp"
#include "RGB.hpp"
#include "constants.hpp"


using namespace std;

struct Ray
{
    Vector3<double> direction;
    Vector3<double> position;
    double length;
    RGB color; // Может быть не только RGB, нужен абстрактый класс
    int count;

    Ray(const Vector3<double> direction = Vector3<double>(1, 0, 0), const Vector3<double> position = Vector3<double>(0, 0, 0), const RGB color = RGB(1, 1, 1), int rr = RAY_REREFRACTIONS);

    // double intersect(const Vector3<double> & a, const Vector3<double> & b) const;
};

#endif