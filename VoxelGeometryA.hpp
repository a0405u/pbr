#ifndef VOXELGEOMETRYA_H
#define VOXELGEOMETRYA_H

#include <iostream>
#include <math.h>

#include "Ray.hpp"
#include "Point.hpp"
#include "Voxel.hpp"
#include "PointGeometry.hpp"

#define BOUNDARY 0.00001

using namespace std;

class VoxelGeometryA: public Geometry
{
    private:

    Voxel *** * voxel; // Массив указателей на воксели

    Vector3<double> position; // Точка начала координат воксельной сетки
    Vector3<double> end; // Точка конца координат воксельной сетки
    Vector3<double> size; // Размер сетки в координатах

    Vector3<unsigned int> resolution; // Размер сетки в вокселях
    unsigned int count; // Количество вокселей
    double scale; // Размер одного вокселя

    public:
    
    VoxelGeometryA(const PointGeometry & cloud, unsigned int voxelsPerMeter);
    ~VoxelGeometryA();

    Voxel * addVoxel(const Vector3<unsigned int> p);
    Voxel * addVoxel(const Vector3<double> p, const Vector3<double> n = Vector3<double>(), const Material * m = nullptr);
    Voxel * addVoxel(const Vector3<unsigned int> p, const Vector3<double> n = Vector3<double>(), const Material * m = nullptr);
    Voxel * getVoxel(const Vector3<unsigned int> & p) const;
    
    Vector3<unsigned int> locate(const Vector3<double> & p) const;
    Vector3<double> getPosition(const Vector3<unsigned int> & p) const;
    bool inside(const Vector3<double> & p) const;
    double intersect(const Ray & ray) const;

    Point * trace(Ray & ray) const;
    void exportPLY(const string filename) const; // Экспорт вокселей в файл PLY
    vector<Point *> toPointGeometry(unsigned int resolution) const {return vector<Point *>();}
};

#endif