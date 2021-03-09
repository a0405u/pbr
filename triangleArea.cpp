#include "triangleArea.hpp"

double cosVector(const Vector3 & a, const Vector3 & b)
{
    return (a * b) / a.length() / b.length();
}

Vector3 vectorMultiply(const Vector3 & a, const Vector3 & b)
{
    return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

double triangleArea(const Vector3 & o, const Vector3 & m, const Vector3 & n)
{
    Vector3 a = m - o;
    Vector3 b = n - o;
            
    double cos = (a * b) / a.length() / b.length();
    double sin = sqrt(1 - cos * cos);
    
    return 0.5 * sin * a.length() * b.length();
}