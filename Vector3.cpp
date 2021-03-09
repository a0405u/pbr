#include "Vector3.hpp"

Vector3::Vector3()
{
    x = 0; y = 0; z = 0;
}

Vector3::Vector3(double a)
{
    x = a; y = a; z = a;
}

Vector3::Vector3(double x, double y, double z)
{
    this->x = x; this->y = y; this->z = z;
}

Vector3::Vector3(string s) // будет работать только для DOUBLE((
{
    istringstream ss(s);
    ss >> x >> y >> z;
    // vector<string> v = getWords(s);

    // x = stod(v[0]);
    // y = stod(v[1]);
    // z = stod(v[2]);
}

Vector3::Vector3(const Vector3 & v)
{
    x = v.x; y = v.y; z = v.z;
}

Vector3 Vector3::operator * (double m) const
{
    return Vector3(x * m, y * m, z * m);
}

Vector3 Vector3::operator / (double m) const
{
    return Vector3(x / m, y / m, z / m);
}

Vector3 Vector3::operator + (const Vector3 & v3) const
{
    return Vector3(x + v3.x, y + v3.y, z + v3.z);
}

Vector3 Vector3::operator - (const Vector3 & v3) const
{
    return Vector3(x - v3.x, y - v3.y, z - v3.z);
}

double Vector3::operator * (const Vector3 & v3) const
{
    return x * v3.x + y * v3.y + z * v3.z;
}

Vector3 Vector3::operator - () const
{
    return Vector3(-x, -y, -z);
}

double Vector3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::normalized() const
{
    return *this / this->length();
}

Vector3 Vector3::rotated(const Vector3 & r) const
{
    return *this;
}