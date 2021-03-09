#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "Scene.hpp"

struct PointCloud
{
    vector<Point *> point;

    PointCloud(const Scene & scene, unsigned int resolution);

    void exportPLY(const string filename) const;
};

#endif