#ifndef CAMERA_H
#define CAMERA_H

#include "Sensor.hpp"
#include "Ray.hpp"

#define CAM_DIR      -1, 0, 0

struct Camera   
{
    Vector3<double> position;
    Vector3<double> direction;
    Vector3<double> rotation;
    Sensor * sensor;

    Camera(Vector3<double> pos, Vector3<double> rot, int resx, int resy, double s, double d)
    {
        position = pos;
        direction = Vector3<double>(CAM_DIR);
        rotation = rot;
        sensor = new Sensor(resx, resy, s, d, pos);
    }

    Ray launch_ray(const Vector3<double> direction);
};

#endif