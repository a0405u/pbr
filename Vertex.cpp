#include "Vertex.hpp"

Vertex::Vertex(double a, double b, double c)
{
    position = Vector3(a, b, c);
}

Vertex::Vertex(string s)
{
    position = Vector3(s);
}

// ostream& operator << (ostream& os, const Vertex& v)
// {
//         os << '[' << v.position.x << "; " << v.position.y << "; " << v.position.z << ']';
//         return os;
// }