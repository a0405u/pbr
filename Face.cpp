#include "Face.hpp"

Face::Face(const string s, const vector<Vertex *> & v, const vector<Vector3 *> & vn) 
{
    vector<string> vs = getWords(s); // Получаем строки типа "v/vt/vn" где vt и vn опциональны

    for (vector<string>::iterator it = vs.begin(); it != vs.end(); ++it) //перебираем все вершины
    {
        vector<string> value = getWords(*it, '/');

        vertex.push_back(v[stoi(value[0]) - 1]);
//        texture.push_back(v[stoi(value[1]) - 1]); координаты текстур
        normal.push_back(vn[stoi(value[2]) - 1]);
    }

    area = triangleArea(vertex[0]->position, vertex[1]->position, vertex[2]->position);
}

double Face::getArea()
{
    area = triangleArea(vertex[0]->position, vertex[1]->position, vertex[2]->position);
    return area;
}

Vector3 Face::getNormal()
{
    Vector3 ab = vertex[1]->position - vertex[0]->position;
    Vector3 ac = vertex[2]->position - vertex[0]->position;

    return vectorMultiply(ab, ac).normalized(); // работает без нормализации???
}

void Face::addVertex(Vertex * v)
{
    vertex.push_back(v);
}

void Face::addVertexNormal(Vector3 * vn)
{
    normal.push_back(vn);
}