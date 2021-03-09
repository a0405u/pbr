#ifndef TRIAREA_H
#define TRIAREA_H

#include "Vector3.hpp"
 
using namespace std;

double cosVector(const Vector3 & a, const Vector3 & b);
double triangleArea(const Vector3 & a, const Vector3 & b, const Vector3 & c);
Vector3 vectorMultiply(const Vector3 & a, const Vector3 & b);

#endif