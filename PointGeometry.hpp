#ifndef POINTGEOMETRY_H
#define POINTGEOMETRY_H

#include "Scene.hpp"

struct PointGeometry
{
    vector<Point *> point;

    PointGeometry(const Scene & scene, unsigned int resolution);

    ~PointGeometry() { for (Point * p : point) delete p; }

    Point* trace(Ray & r) const { return nullptr; } // Трассировка луча в облаке точек

    void sort(); // Сортировка облака точек по возрастанию
    vector<Point *> convert(const Geometry & geometry, unsigned int resolution) const;
    vector<Point *> convert(const PolygonalGeometry & geometry, unsigned int resolution) const;
    void exportPLY(const string filename) const; // Экспорт облака в файл PLY
    vector<Point *> toPointGeometry(unsigned int resolution) const {return vector<Point *>();}
};

#endif