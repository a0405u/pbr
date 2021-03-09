#include "PointCloud.hpp"

PointCloud::PointCloud(const Scene & scene, unsigned int resolution)
{
    vector<Point *> p;

    for (unsigned long i = 0; i < scene.geometry.size(); ++i)
    {
        p = scene.geometry[i]->toPointCloud(resolution);

        point.reserve(point.size() + p.size());
        point.insert(point.end(), p.begin(), p.end());
    }
}

void PointCloud::exportPLY(const string filename) const
{
    ofstream file(filename);

    if (file.is_open()) 
    {
        file << "ply" << '\n';
        file << "format ascii 1.0" << '\n';
        file << "element vertex " << point.size() << '\n';
        file << "property float x" << '\n';
        file << "property float y" << '\n';
        file << "property float z" << '\n';
        file << "property float nx" << '\n';
        file << "property float ny" << '\n';
        file << "property float nz" << '\n';
        file << "property uchar red" << '\n';
        file << "property uchar green" << '\n';
        file << "property uchar blue" << '\n';
        file << "end_header" << '\n';

        RGB color;

        for (const Point * p : point)
        {
            color = p->material->getColor() * 255;
            
            file << (float)p->position.x << ' ' << (float)p->position.y << ' ' << (float)p->position.z << ' ';
            file << (float)p->normal.x << ' ' << (float)p->normal.y << ' ' << (float)p->normal.z << ' ';
            file << (int)color.r << ' ' << (int)color.g << ' ' << (int)color.b << '\n';
        }
    }
    else
        throw "Incorrect file path!";
    file.close();
}