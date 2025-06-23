#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <vector>
#include <iostream>

#include "Headers.hpp"
#include "Vector3.hpp"

using namespace std;

struct Vertex
{
    Vector3<double> position;

    Vertex(){};
    Vertex(double a, double b, double c);
    Vertex(string s);

    // friend ostream& operator << (ostream& os, const Vertex& v);

};

#endif