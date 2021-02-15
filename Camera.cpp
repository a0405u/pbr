#include "Camera.hpp"

Ray Camera::launch_ray(const Vector3 direction)
{
    return Ray(position, direction);
}