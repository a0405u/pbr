#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

#include "Ray.hpp"
#include "Point.hpp"

using namespace std;

struct Geometry
{
    Geometry() { return; }

    virtual Point* trace(Ray & r) = 0;
};

#endif