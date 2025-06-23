#include "triangleArea.hpp"

double cosVector(const Vector3<double> & a, const Vector3<double> & b)
{
    return (a * b) / a.length() / b.length();
}

Vector3<double> vectorMultiply(const Vector3<double> & a, const Vector3<double> & b)
{
    return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

double triangleArea(const Vector3<double> & o, const Vector3<double> & m, const Vector3<double> & n)
{
    Vector3 a = m - o;
    Vector3 b = n - o;
            
    double cos = (a * b) / a.length() / b.length();
    double sin = sqrt(1 - cos * cos);
    
    return 0.5 * sin * a.length() * b.length();
}