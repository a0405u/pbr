#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>

#include "Ray.hpp"
#include "Point.hpp"

using namespace std;

struct Geometry
{
    Geometry() { return; }

    virtual ~Geometry() { return; }

    virtual Point* trace(Ray & r) const = 0;

    virtual vector<Point *> toPointGeometry(unsigned int resolution) const = 0;
};

#endif