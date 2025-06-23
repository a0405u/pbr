#include "PointLight.hpp"

PointLight::PointLight()
{
    flux = 0;
}

PointLight::PointLight(double f)
{
    flux = f;
}

PointLight::PointLight(double f, Vector3<double> pos)
{
    flux = f; position = pos;
}

PointLight::PointLight(double f, Vector3<double> pos, RGB col)
{
    flux = f; position = pos; color = col;
}

RGB PointLight::getIntencity(Vector3<double> n) const
{
    return color * (flux / 4 / M_PI);
}