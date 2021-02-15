#ifndef OBJ_H
#define OBJ_H

#include <vector>

#include "vertex.hpp"
#include "face.hpp"
#include "fstream"

using namespace std;

struct Obj
{
    vector<Vertex *> v;
    vector<Face *> f;
    bool s;

    Obj(string filename);
    
};

#endif