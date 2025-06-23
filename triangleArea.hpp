#ifndef TRIAREA_H
#define TRIAREA_H

#include "Vector3.hpp"
 
using namespace std;

double cosVector(const Vector3<double> & a, const Vector3<double> & b);
double triangleArea(const Vector3<double> & a, const Vector3<double> & b, const Vector3<double> & c);
Vector3<double> vectorMultiply(const Vector3<double> & a, const Vector3<double> & b);

#endif