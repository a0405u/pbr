#include "PointLight.hpp"

PointLight::PointLight()
{
    flux = 0;
}

PointLight::PointLight(double f)
{
    flux = f;
}

PointLight::PointLight(double f, Vector3 pos)
{
    flux = f; position = pos;
}

PointLight::PointLight(double f, Vector3 pos, RGB col)
{
    flux = f; position = pos; color = col;
}

RGB PointLight::getIntencity(Vector3 n) const
{
    return color * (flux / 4 / M_PI);
}