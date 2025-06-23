#ifndef VECTOR3_H
#define VECTOR3_H

#include <string>
#include <math.h>

#include "Headers.hpp"

using namespace std;


template <typename Atype = double> struct Vector3;
template <typename Atype> Vector3<Atype> operator / (const Atype m, const Vector3<Atype> & v);

template <typename Atype> struct Vector3
{
        Atype x, y, z;

        Vector3() {x = 0; y = 0; z = 0; }
        Vector3(Atype a) { x = a; y = a; z = a; }
        Vector3(Atype x, Atype y, Atype z) { this->x = x; this->y = y; this->z = z; }
        Vector3(string s);
        Vector3(const Vector3<Atype> & v);

        template <typename Btype> Vector3<Atype> operator * (Btype m) const;
        Vector3<Atype> operator / (Atype m) const;
        template <typename Btype> Vector3<Atype> operator + (const Vector3<Btype> & v) const;
        template <typename Btype> Vector3<Atype> operator + (Btype m) const;
        Vector3<Atype> operator - (const Vector3<Atype> & v) const;
        Vector3<Atype> operator / (const Vector3<Atype> & v) const;
        double operator * (const Vector3<Atype> & v) const;
        Vector3<Atype> operator - () const;
        bool operator > (const Vector3<Atype> & v) const;
        bool operator > (Atype m) const;
        bool operator < (Atype m) const;
        bool operator == (const Vector3<Atype> & v) const;
        template <typename Btype> operator Vector3<Btype>() const;
        Vector3<Atype> & operator += (const Vector3<Atype> & v);
        Vector3<Atype> & operator /= (Atype m);
        Atype & operator [] (int i);
        const Atype & operator [] (int i) const;

        double length() const;
        Atype max() const;
        Vector3<Atype> normalized() const;
        Vector3<Atype> normalize();
        Vector3<Atype> abs() const;
        Vector3<Atype> rotated(const Vector3<Atype> & r) const;
};

#endif