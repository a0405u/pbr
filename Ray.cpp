#include "Ray.hpp"

Ray::Ray(const Vector3<double> direction, const Vector3<double> position, const RGB color, int rr)
{
    this->direction = direction.normalized();
    this->position = position;
    this->color = color;
    this->length = RAY_CLIP;
    this->count = rr;
}

// // Пересечения луча с Axis Aligned Box
// // Возвращает отрицательные значения в случае ненахождения точки пересечения
// double Ray::intersect(const Vector3<double> & a, const Vector3<double> & b) const
// {
//     Vector3<double> bound[2] = {a, b};
//     Vector3<double> invDir = Vector3<double>(1 / direction.x, 1 / direction.y, 1 / direction.z);
//     Vector3<double> tmin, tmax;

//     tmin.x = (bound[invDir.x < 0].x - position.x) * invDir.x; 
//     tmax.x = (bound[1 - invDir.x < 0].x - position.x) * invDir.x; 
//     tmin.y = (bound[invDir.y < 0].y - position.y) * invDir.y; 
//     tmax.y = (bound[1 - invDir.y < 0].y - position.y) * invDir.y; 

//     if ((tmin.x > tmax.y) or (tmin.y > tmax.x)) 
//         return -1; 

//     if (tmin.y > tmin.x) 
//         tmin.x = tmin.y; 
//     if (tmax.y < tmax.x) 
//         tmax.x = tmax.y; 

//     tmin.z = (bound[invDir.z < 0].z - position.z) * invDir.z; 
//     tmax.z = (bound[1 - invDir.z < 0].z - position.z) * invDir.z; 

//     if ((tmin.x > tmax.z) or (tmin.z > tmax.x)) 
//         return -1; 

//     if (tmin.z > tmin.x) 
//         tmin.x = tmin.z; 
//     if (tmax.z < tmax.x) 
//         tmax.x = tmax.z; 

//     if (tmin.x < 0)
//         return tmax.x; 
//     else
//         return tmin.x;
// }