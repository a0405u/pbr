#include "Camera.hpp"

Ray Camera::launch_ray(const Vector3<double> direction)
{
    return Ray(position, direction);
}