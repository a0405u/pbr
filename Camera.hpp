#ifndef CAMERA_H
#define CAMERA_H

#include "Sensor.hpp"
#include "Ray.hpp"

#define CAM_DIR      -1, 0, 0

struct Camera   
{
    Vector3 position;
    Vector3 direction;
    Vector3 rotation;
    Sensor * sensor;

    Camera(Vector3 pos, Vector3 rot, int resx, int resy, double s, double d)
    {
        position = pos;
        direction = Vector3 (CAM_DIR);
        rotation = rot;
        sensor = new Sensor(resx, resy, s, d, pos);
    }

    Ray launch_ray(const Vector3 direction);
};

#endif