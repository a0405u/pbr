#include "PointGeometry.hpp"

PointGeometry::PointGeometry(const Scene & scene, unsigned int resolution)
{
    vector<Point *> p;

    for (unsigned long i = 0; i < scene.geometry.size(); ++i)
    {
        p = scene.geometry[i]->toPointGeometry(resolution);

        point.reserve(point.size() + p.size());
        point.insert(point.end(), p.begin(), p.end());
    }
}

void PointGeometry::sort()
{
    Point * temp;

    for(unsigned int i = 1; i < point.size(); i++)     
	    for(unsigned int j = i; j > 0 && point[j-1]->position > point[j]->position; j--)
        {
            temp = point[j];
            point[j] = point[j-1];
            point[j-1] = temp;
        }
}

vector<Point *> PointGeometry::convert(const Geometry & geometry, unsigned int resolution) const
{
    return vector<Point *>();
}

vector<Point *> PointGeometry::convert(const PolygonalGeometry & geometry, unsigned int resolution) const
{
    unsigned int n;
    vector<Point *> points;

    for (unsigned long i = 0; i < geometry.face.size(); ++i) // Последовательный проход по всем треугольникам геометрии
    {
        n = geometry.face[i]->area * resolution; // Количество точек в треугольнике

        // Вершины обрабатываемого треугольника
        Vector3<double> & a = geometry.face[i]->vertex[0]->position;
        Vector3<double> & b = geometry.face[i]->vertex[1]->position;
        Vector3<double> & c = geometry.face[i]->vertex[2]->position;

        // Нормали в вершинах треугольника
        Vector3<double> & an = *geometry.face[i]->normal[0];
        Vector3<double> & bn = *geometry.face[i]->normal[1];
        Vector3<double> & cn = *geometry.face[i]->normal[2];
        
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
            cw = triangleArea(position, a, b) / geometry.face[i]->area;
            aw = triangleArea(position, b, c) / geometry.face[i]->area;
            bw = triangleArea(position, c, a) / geometry.face[i]->area;

            normal = an * aw + bn * bw + cn * cw; // Нормаль в точке

            points.push_back(new Point(position, normal, geometry.material)); // Созданием объекта точки и помещение в массив
            
            n--; // Переход к следующей
        }
    }

    return points;
}

void PointGeometry::exportPLY(const string filename) const
{
    ofstream file(filename);

    if (file.is_open()) 
    {
        file << "ply" << '\n';
        file << "format ascii 1.0" << '\n';
        file << "element vertex " << point.size() << '\n';
        file << "property float x" << '\n';
        file << "property float y" << '\n';
        file << "property float z" << '\n';
        file << "property float nx" << '\n';
        file << "property float ny" << '\n';
        file << "property float nz" << '\n';
        file << "property uchar red" << '\n';
        file << "property uchar green" << '\n';
        file << "property uchar blue" << '\n';
        file << "end_header" << '\n';

        RGB color;

        for (const Point * p : point)
        {
            color = p->material->getDiffuse() * 255;
            
            file << (float)p->position.x << ' ' << (float)p->position.y << ' ' << (float)p->position.z << ' ';
            file << (float)p->normal.x << ' ' << (float)p->normal.y << ' ' << (float)p->normal.z << ' ';
            file << (int)color.r << ' ' << (int)color.g << ' ' << (int)color.b << '\n';
        }
    }
    else
        throw "Incorrect file path!";
    file.close();
}