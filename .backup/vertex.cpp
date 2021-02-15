#include "vertex.hpp"

using namespace std;

Vertex::Vertex(string s)
{
    vector<string> v = getwords(s, ' ');

    x = stod(v[0]);
    y = stod(v[1]);
    z = stod(v[2]);
}

ostream& operator << (ostream& os, const Vertex& v)
    {
        os << '[' << v.x << "; " << v.y << "; " << v.z << ']';
        return os;
    }