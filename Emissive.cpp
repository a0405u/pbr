#include "Emissive.hpp"
#include <iostream>

RGB Emissive::luminance(Point p, Ray o, const Scene & scene)
{
    if (o.count == RAY_REREFRACTIONS)
        return color * i / cosVector(p.normal, -o.direction) / o.length / o.length; // Возвращение яркости
    return RGB();
}