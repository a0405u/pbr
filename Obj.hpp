#ifndef OBJ_H
#define OBJ_H

#include <fstream>

#include "Geometry.hpp"
#include "Vector3.hpp"
#include "Face.hpp"
#include "PointLight.hpp"

using namespace std;

struct Obj
{
    vector<Vertex *> v; // вершины
    vector<Vector3<double> *> vn; // нормали
    vector<Face *> f; // полигоны
    vector<PointLight *> l;
    RGB material;
    bool s;

    Obj(string filename);
};

#endif