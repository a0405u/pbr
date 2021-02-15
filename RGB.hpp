#ifndef RGB_H
#define RGB_H

#define PRECISION   0.0001

struct RGB // можно использовать вектор, но...
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
    bool operator != (double d) const;
};

#endif