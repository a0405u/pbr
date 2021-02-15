#ifndef VECTOR3_H
#define VECTOR3_H

#include "vector2.h"

template <typename Ttype> struct Vector3
{
        Ttype x, y ,z;

        Vector3();
        Vector3(Ttype a);
        Vector3(Ttype x, Ttype y, Ttype z);

        Vector3<Ttype> operator*(Ttype m);
        template <typename Ctype> Vector3<Ttype> operator+(Vector2<Ctype> v);
};

#endif