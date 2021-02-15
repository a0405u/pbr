#ifndef VECTOR2_H
#define VECTOR2_H

template <typename Ttype> struct Vector2
{
        Ttype x, y;

        Vector2();
        Vector2(Ttype a);
        Vector2(Ttype x, Ttype y);

        Vector2<Ttype> operator*(Ttype m);
};

#endif