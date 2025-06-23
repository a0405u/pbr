#include "RGB.hpp"

RGB::RGB()
{
    r = 0; g = 0; b = 0;
}

RGB::RGB(double r, double g, double b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

RGB RGB::operator * (double m) const
{
    return RGB(r * m, g * m, b * m); 
}

RGB RGB::operator / (double m) const
{
    return RGB(r / m, g / m, b / m); ;
}

RGB RGB::operator * (RGB c) const
{
    return RGB(r * c.r, g * c.g, b * c.b); 
}

RGB RGB::operator + (RGB c) const
{
    return RGB(r + c.r, g + c.g, b + c.b); 
}

RGB & RGB::operator += (const RGB & c)
{
    this->r += c.r; 
    this->g += c.g; 
    this->b += c.b; 

    return *this;
}

RGB & RGB::operator /= (double m)
{
    r /= m; 
    g /= m; 
    b /= m; 

    return *this;
}

bool RGB::operator != (double d) const
{
    if (r < d + PRECISION and r > d - PRECISION)
        if (g < d + PRECISION and g > d - PRECISION)
            if (b < d + PRECISION and b > d - PRECISION)
                return false;
    return true;
}