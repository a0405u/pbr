#ifndef RAY_H
#define RAY_H

#include "Vector3.hpp"
#include "RGB.hpp"

#define RAY_CLIP            100
#define RAY_REREFRACTIONS   6
#define PRECISION           0.0001

using namespace std;

struct Ray
{
    Vector3 direction;
    Vector3 position;
    double length;
    RGB color; // Может быть не только RGB, нужен абстрактый класс
    int count;

    Ray(const Vector3 direction = Vector3(1, 0, 0), const Vector3 position = Vector3(0, 0, 0), const RGB color = RGB(1, 1, 1), int rr = RAY_REREFRACTIONS);
};

#endif