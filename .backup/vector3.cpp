#include "vector3.h"

template <typename Ttype>
Vector3<Ttype>::Vector3()
{
    x = 0; y = 0; z = 0;
}

template <typename Ttype>
Vector3<Ttype>::Vector3(Ttype a)
{
    x = a; y = a; z = a;
}

template <typename Ttype>
Vector3<Ttype>::Vector3(Ttype x, Ttype y, Ttype z)
{
    this->x = x; this->y = y; this->z = z;
}

template <typename Ttype>
Vector3<Ttype> Vector3<Ttype>::operator *(Ttype m)
{
    Vector3<Ttype> v3(x * m, y * m, z * m); 

    return v3;
}

template <typename Ttype>
template <typename Ctype> 
Vector3<Ttype> Vector3<Ttype>::operator +(Vector2<Ctype> v2)
{
    Vector3<Ttype> v3(x + v2.x, y + v2.y, z); 

    return v3;
}