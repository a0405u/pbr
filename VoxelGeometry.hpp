#ifndef VOXELGEOMETRY_H
#define VOXELGEOMETRY_H

#include <iostream>
#include <math.h>

#include "Ray.hpp"
#include "Point.hpp"
#include "Voxel.hpp"
#include "Octree.hpp"
#include "PointGeometry.hpp"

#define BOUNDARY 0.0000000000001

using namespace std;

class VoxelGeometry: public Geometry
{
    private:

    Octree * tree; // Октодерево вокселей

    Vector3<double> position; // Точка начала координат воксельной сетки
    Vector3<double> end; // Точка конца координат воксельной сетки
    Vector3<double> size; // Размер сетки в координатах

    Vector3<unsigned int> resolution; // Размер сетки в вокселях
    unsigned int total; // Количество вокселей
    double scale; // Размер одного вокселя

    public:
    
    VoxelGeometry(const PointGeometry & cloud, unsigned int voxelsPerMeter);
    ~VoxelGeometry();

    // Voxel * addVoxel(const Vector3<unsigned int> p);
    // Voxel * addVoxel(const Vector3<double> p, const Vector3<double> n = Vector3<double>(), const Material * m = nullptr);
    // Voxel * addVoxel(const Vector3<unsigned int> p, const Vector3<double> n = Vector3<double>(), const Material * m = nullptr);
    // Voxel * getVoxel(const Vector3<unsigned int> & p) const;
    
    Octree * createVoxel(Octree * parent, Vector3<unsigned int> p, unsigned int dr, Voxel *** * voxel);
    bool inside(const Vector3<double> & p) const;
    double intersect(const Ray & ray) const;

    Vector3<unsigned int> locate(const Vector3<double> & p) const;
    Vector3<double> getPosition(const Vector3<unsigned int> & p) const;
    unsigned int getId(unsigned int x, unsigned int y, unsigned int z) const;

    Point * trace(Ray & ray) const;
    Point * trace(Ray & ray, const Octree * parent, Vector3<double> parentPosition, double depthScale, double length) const;

    unsigned int count(const Octree * parent) const;
    void printPLY(ofstream & file, const Octree * parent, Vector3<unsigned int> p, unsigned int dr) const;
    void exportPLY(const string filename) const; // Экспорт вокселей в файл PLY
    vector<Point *> toPointGeometry(unsigned int resolution) const {return vector<Point *>();}

    Vector3<double> getPosition(const Vector3<int> & voxelPosition, const double & size) const;
    Vector3<double> getPosition(const Vector3<double> & position, const double & size) const;

    Vector3<int> getVoxelPosition(const Vector3<double> & position, const double & size) const;

    Vector3<double> getT(const Vector3<double> & bound, const Vector3<double> & tracerPosition, const Vector3<double> & rayDirection) const;
};

#endif