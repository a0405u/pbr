#ifndef FACE_H
#define FACE_H

#include <vector>

#include "Vertex.hpp"
#include "Headers.hpp"
#include "triangleArea.hpp"

using namespace std;

struct Face
{
    vector<Vertex *> vertex; 
    vector<Vector3<double> *> normal;
    double area;

    Face(){};
    Face(const string s, const vector<Vertex *> & v, const vector<Vector3<double> *> & vn);

    double getArea();
    Vector3<double> getNormal();
    void addVertex(Vertex * v);
    void addVertexNormal(Vector3<double> * vn);

//    friend ostream& operator << (ostream& os, const Face& f);
};

#endif