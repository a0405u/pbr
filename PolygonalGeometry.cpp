#include "PolygonalGeometry.hpp"
#include "Lambert.hpp"

PolygonalGeometry::PolygonalGeometry()
{
    material = new Lambert(RGB(1, 1, 1));
    smooth = false;
}

void PolygonalGeometry::addVertex(Vertex * v)
{
    vertex.push_back(v);
}

void PolygonalGeometry::addVertexNormal(Vector3 * vn)
{
    vertexNormal.push_back(vn);
}

void PolygonalGeometry::addFace(Face * f)
{
    face.push_back(f);
}

int PolygonalGeometry::sizeVertex() const
{
    return vertex.size();
}

int PolygonalGeometry::sizeVertexNormal() const
{
    return vertexNormal.size();
}

int PolygonalGeometry::sizeFace() const
{
    return face.size();
}

bool PolygonalGeometry::empty()
{
    return vertex.empty();
}

Point * PolygonalGeometry::trace(Ray & r)
{
    double len;
    double mostlen = r.length;
    Vector3 mostpoint;
    Vector3 mostnormal;

    for (unsigned long i = 0; i < face.size(); ++i)
    {
        //Vector3 & normal = *f[i]->normal[0]; // нужно вычислить относительно точек треугольника
        Vector3 & a = face[i]->vertex[0]->position;
        Vector3 & b = face[i]->vertex[1]->position;
        Vector3 & c = face[i]->vertex[2]->position;

        Vector3 ab = b - a;
        Vector3 ac = c - a;

        Vector3 normal = vectorMultiply(ab, ac).normalized(); // работает без нормализации???

        double dn = r.direction * normal;

        if (dn > PRECISION || dn < -PRECISION) // проверка на параллельность
        {

            Vector3 &an = *face[i]->normal[0];
            Vector3 &bn = *face[i]->normal[1];
            Vector3 &cn = *face[i]->normal[2];

            len = (face[i]->vertex[0]->position * normal - r.position * normal) / (dn);
            Vector3 point = r.position + r.direction * len;

            double cw = triangleArea(point, a, b) / face[i]->area; //Барицентрические координаты
            double aw = triangleArea(point, b, c) / face[i]->area;
            double bw = triangleArea(point, c, a) / face[i]->area;

            if (aw + bw + cw <= 1 + PRECISION) // Проверка на принадлежность треугольнику
            {
                if (len > 0 and len < mostlen)
                {
                    mostlen = len;
                    mostpoint = point;
                    mostnormal = an * aw + bn * bw + cn * cw; // Нормаль в точке попадания луча
//                    value = RGB(len, len, len); //расстояние до точки
                }
            }
        }
    }

    if(mostlen < r.length) // Если была найдена точка пересечения луча и геометрии
    {
        r.length = mostlen;
        return new Point(mostpoint, mostnormal, material);
    }

    return NULL;
}

vector<Point *> PolygonalGeometry::toPointCloud(unsigned int resolution) const
{
    unsigned int n;
    vector<Point *> points;

    for (unsigned long i = 0; i < face.size(); ++i) // Последователньый проход по всем треугольникам геометрии
    {
        n = face[i]->area * resolution; // Количество точек в треугольнике

        // Вершины обрабатываемого треугольника
        Vector3 & a = face[i]->vertex[0]->position;
        Vector3 & b = face[i]->vertex[1]->position;
        Vector3 & c = face[i]->vertex[2]->position;

        // Нормали в вершинах треугольника
        Vector3 &an = *face[i]->normal[0];
        Vector3 &bn = *face[i]->normal[1];
        Vector3 &cn = *face[i]->normal[2];
        
        // Вектора из одной точки составляющие треугольник
        Vector3 ab = b - a;
        Vector3 ac = c - a;

        double k, l, cw, aw, bw;
        Vector3 position;
        Vector3 normal;

        while (n > 0) // Создаем n точек
        {
            //Два случайных числа-координаты
            k = (double)rand() / RAND_MAX;
            l = (double)rand() / RAND_MAX;

            if (k + l > 1) // Если выходим за границы треугольника в параллелограм
                k = 1 - k, l = 1 - l; // Переворачиваем координаты внутрь треуголника
            
            position = a + (ab * k) + (ac * l); // Координаты случайной точки

            //Барицентрические координаты точки в треугольнике
            cw = triangleArea(position, a, b) / face[i]->area;
            aw = triangleArea(position, b, c) / face[i]->area;
            bw = triangleArea(position, c, a) / face[i]->area;

            normal = an * aw + bn * bw + cn * cw; // Нормаль в точке

            points.push_back(new Point(position, normal, material)); // Созданием объекта точки и помещение в массив
            
            n--; // Переход к следующей
        }
    }

    return points;
}