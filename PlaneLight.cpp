#include "PlaneLight.hpp"


PlaneLight::PlaneLight(double i, Vector3 pos, double sx, double sy, Vector3 r, RGB col)
{
    intencity = i;
    position = pos;
    rotation = r;
    color = col;
    sizex = sx;
    sizey = sy;
    normal = Vector3(0, 0, -1).rotated(rotation);
}

RGB PlaneLight::getIntencity(Vector3 n) const
{
    double cos = cosVector(normal, n);

    if (cos <= 0)
        return RGB(0, 0, 0);

    return color * intencity * cos;
}

Vector3 PlaneLight::getPosition() const
{
    double x = random(0, sizex);
    double y = random(0, sizey);

    return position + Vector3(x, y, 0).rotated(rotation); 
}