#ifndef RGB_H
#define RGB_H

#include "Color.hpp"
#include "Constants.hpp"

//#define PRECISION   std::numeric_limits<double>::epsilon()

struct RGB: public Color
{
    double r;
    double g;
    double b;

    RGB();
    RGB(double r, double g, double b);

    RGB operator * (double m) const;
    RGB operator / (double m) const;
    RGB operator * (RGB c) const;
    RGB operator + (RGB c) const;
    RGB & operator += (const RGB & c);
    RGB & operator /= (double m);
    bool operator != (double d) const;
};

#endif