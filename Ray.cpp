#include "Ray.hpp"

Ray::Ray(const Vector3 direction, const Vector3 position, const RGB color, int rr)
{
    this->direction = direction.normalized();
    this->position = position;
    this->color = color;
    this->length = RAY_CLIP;
    this->count = rr;
}