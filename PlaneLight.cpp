#include "PlaneLight.hpp"
#include "Emissive.hpp"


PlaneLight::PlaneLight(double i, Vector3<double> pos, double sx, double sy, Vector3<double> r, RGB col, Material * m)
{
    intencity = i;
    position = pos;
    rotation = r;
    color = col;
    sizex = sx;
    sizey = sy;
    normal = Vector3<double>(0, 0, -1).rotated(rotation);

    if (not m)
        material = new Emissive(col, i / sizex / sizey);
}

Point* PlaneLight::trace(Ray & ray) const
{
    double t = - ((ray.position - position) * normal) / (ray.direction * normal);

    if (t > ray.length)
        return NULL;

    Vector3<double> p = ray.position + ray.direction * t;

    if (p.x < position.x or p.x > position.x + sizex)
        return NULL;
    
    if (p.y < position.y or p.y > position.y + sizey)
        return NULL;

    ray.length = t;
    return new Point(p, normal, material);
}

RGB PlaneLight::getIntencity(Vector3<double> n) const
{
    double cos = cosVector(normal, n);

    if (cos <= 0)
        return RGB(0, 0, 0);

    return color * intencity * cos;
}

Vector3<double> PlaneLight::getPosition() const
{
    double x = random<double>(0, sizex);
    double y = random<double>(0, sizey);

    return position + Vector3<double>(x, y, 0).rotated(rotation); 
}