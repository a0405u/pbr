#ifndef POLYGONALGEOMETRY_H
#define POLYGONALGEOMETRY_H

#include <fstream>
#include <tuple>

#include "Geometry.hpp"
#include "Material.hpp"
#include "Point.hpp"
#include "Vector3.hpp"
#include "Face.hpp"
#include "RGB.hpp"

using namespace std;

struct PolygonalGeometry: public Geometry
{
    vector<Vertex *> vertex; // Вершины
    vector<Vector3<double> *> vertexNormal; // Нормали
    vector<Face *> face; // Полигоны
    Material * material;
    bool smooth; // Сглаженность поверхности

    PolygonalGeometry();
    ~PolygonalGeometry();

    void addVertex(Vertex * v);
    void addVertexNormal(Vector3<double> * vn);
    void addFace(Face * f);
    void addMaterial(Material * m);
    int sizeVertex() const;
    int sizeVertexNormal() const;
    int sizeFace() const;
    bool empty();
    Point* trace(Ray & r) const;

    vector<Point *> toPointGeometry(unsigned int resolution) const;
};

#endif