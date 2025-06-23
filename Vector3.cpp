#include "Vector3.hpp"
#include "Headers.hpp"

template <typename Atype>
Vector3<Atype>::Vector3(string s) // будет работать только для DOUBLE((
{
    istringstream ss(s);
    ss >> x >> y >> z;
    // vector<string> v = getWords(s);

    // x = stod(v[0]);
    // y = stod(v[1]);
    // z = stod(v[2]);
}

template <typename Atype>
Vector3<Atype>::Vector3(const Vector3 & v)
{
    x = v.x; y = v.y; z = v.z;
}

template <typename Atype>
template <typename Btype>
Vector3<Atype> Vector3<Atype>::operator * (Btype m) const
{
    return Vector3<Atype>(x * m, y * m, z * m);
}

template <typename Atype>
Vector3<Atype> Vector3<Atype>::operator / (Atype m) const
{
    return Vector3<Atype>(x / m, y / m, z / m);
}

template <typename Atype>
template <typename Btype>
Vector3<Atype> Vector3<Atype>::operator + (const Vector3<Btype> & v) const
{
    return Vector3<Atype>(x + v.x, y + v.y, z + v.z);
}

template <typename Atype>
template <typename Btype> 
Vector3<Atype> Vector3<Atype>::operator + (Btype m) const
{
    return Vector3<Atype>(x + m, y + m, z + m);
}

template <typename Atype>
Vector3<Atype> Vector3<Atype>::operator / (const Vector3 & v) const
{
    return Vector3<Atype>(x / v.x, y / v.y, z / v.z);
}

template <typename Atype>
Vector3<Atype> Vector3<Atype>::operator - (const Vector3 & v3) const
{
    return Vector3<Atype>(x - v3.x, y - v3.y, z - v3.z);
}

template <typename Atype>
double Vector3<Atype>::operator * (const Vector3<Atype> & v3) const
{
    return x * v3.x + y * v3.y + z * v3.z;
}

template <typename Atype>
Vector3<Atype> Vector3<Atype>::operator - () const
{
    return Vector3<Atype>(-x, -y, -z);
}

template <typename Atype>
bool Vector3<Atype>::operator > (const Vector3 & v) const
{
    return x > v.x or y > v.y or z > v.z;
}

template <typename Atype>
bool Vector3<Atype>::operator > (Atype m) const
{
    return x > m or y > m or z > m;
}

template <typename Atype>
bool Vector3<Atype>::operator < (Atype m) const
{
    return x < m or y < m or z < m;
}

template <typename Atype>
bool Vector3<Atype>::operator == (const Vector3 & v) const
{
    return x == v.x and y == v.y and z == v.z;
}

template <typename Atype>
Vector3<Atype> & Vector3<Atype>::operator += (const Vector3<Atype> & v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

template <typename Atype>
Vector3<Atype> & Vector3<Atype>::operator /= (Atype m)
{
    x /= m;
    y /= m;
    z /= m;
    return *this;
}

template <typename Atype>
Atype & Vector3<Atype>::operator [] (int i)
{
    return (&x)[i];
}

template <typename Atype>
const Atype & Vector3<Atype>::operator [] (int i) const
{
    return (&x)[i];
}


template <typename Atype>
template <typename Btype> 
Vector3<Atype>::operator Vector3<Btype>() const
{
    return Vector3<Btype>(
        static_cast<Btype>(this->x),
        static_cast<Btype>(this->y),
        static_cast<Btype>(this->z)
    );
}

template <typename Atype>
double Vector3<Atype>::length() const
{
    return sqrt(x * x + y * y + z * z);
}

template <typename Atype>
Vector3<Atype> Vector3<Atype>::normalized() const
{
    return *this / this->length();
}

template <typename Atype>
Vector3<Atype> Vector3<Atype>::normalize()
{
    double length = this->length();

    if(length != 0.0)
    {
        x /= length;
        y /= length;
        z /= length;
    }
    
    return *this;
}

template <typename Atype>
Vector3<Atype> Vector3<Atype>::abs() const
{
    return Vector3<Atype>(std::abs(double(x)), std::abs(double(y)), std::abs(double(z)));
}

template <typename Atype>
Atype Vector3<Atype>::max() const
{
    Atype max = x;
    if (y > max) max = y;
    if (z > max) max = z;
    return max;
}

template <typename Atype>
Vector3<Atype> Vector3<Atype>::rotated(const Vector3<Atype> & r) const
{
    return *this;
}

template struct Vector3< int >;
template struct Vector3< float >;
template struct Vector3< double >;
template struct Vector3< unsigned int >;

template Vector3<double> Vector3<double>::operator * (double m) const;
template Vector3<double> Vector3<double>::operator * (int m) const;
template Vector3<double> Vector3<double>::operator + (const Vector3<double> & v) const;
template Vector3<double> Vector3<double>::operator + (const Vector3<int> & v) const;
template Vector3<unsigned int> Vector3<unsigned int>::operator + (const Vector3<unsigned int> & v) const;
template Vector3<double> Vector3<double>::operator + (double m) const;
template Vector3<double> Vector3<double>::operator + (int m) const;

template <typename Atype>
Vector3<Atype> operator / (const Atype m, const Vector3<Atype> & v)
{
    return Vector3<Atype>(m / v.x, m / v.y, m / v.z);
}

template Vector3<double> operator / <double> (const double m, const Vector3<double> & v);