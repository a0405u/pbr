#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <sstream>

#include "headers.hpp"

using namespace std;

struct Vertex
{
    double x, y, z; // Vector3?

    Vertex(){};
    Vertex(double a, double b, double c) {x = a; y = b; z = c;}
    Vertex(string s);

    friend ostream& operator << (ostream& os, const Vertex& v);

};

#endif