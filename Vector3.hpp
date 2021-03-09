#ifndef VECTOR3_H
#define VECTOR3_H

#include <string>
#include <math.h>

#include "Headers.hpp"

using namespace std;

struct Vector3
{
        double x, y ,z;

        Vector3();
        Vector3(double a);
        Vector3(double x, double y, double z);
        Vector3(string s);
        Vector3(const Vector3 & v);

        Vector3 operator * (double m) const;
        Vector3 operator / (double m) const;
        Vector3 operator + (const Vector3 & v) const;
        Vector3 operator - (const Vector3 & v) const;
        double operator * (const Vector3 & v) const;
        Vector3 operator - () const;

        double length() const;
        Vector3 normalized() const;
        Vector3 rotated(const Vector3 & r) const;
};

#endif