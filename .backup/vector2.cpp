#include "vector2.h"

template <typename Ttype>
Vector2<Ttype>::Vector2()
{
    x = 0; y = 0;
}

template <typename Ttype>
Vector2<Ttype>::Vector2(Ttype a)
{
    x = a; y = a;
}

template <typename Ttype>
Vector2<Ttype>::Vector2(Ttype x, Ttype y)
{
    this->x = x; this->y = y;
}

template <typename Ttype>
Vector2<Ttype> Vector2<Ttype>::operator *(Ttype m)
{
    Vector2<Ttype> v2(x * m, y * m);

    return v2;
}