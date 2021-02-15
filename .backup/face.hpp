#ifndef FACE_H
#define FACE_H

#include <vector>

#include "vertex.hpp"
#include "headers.hpp"

using namespace std;

struct Face
{
    vector<Vertex *> v; 

    Face(){};
    Face(const string s, const vector<Vertex *> & vp);

//    friend ostream& operator << (ostream& os, const Face& f);
};

#endif