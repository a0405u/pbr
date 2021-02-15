#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>

#include "Headers.hpp"
#include "Obj.hpp"
#include "Vector3.hpp"
#include "triangleArea.hpp"

#define OBJF "Scenes/ICOsphere.obj"
#define OUTPUT "Render/ICOout.txt"

#define CAM_POS      4, 0, 0
#define CAM_DIR      -1, 0, 0
#define CAM_ROT      0, 0, 0
#define CLIP         100;

#define SNS_RESX     320
#define SNS_RESY     240
#define SNS_SCALE    0.007
#define SNS_DIST     0.5

#define PRECISION 0.0001

using namespace std;

/*  
— — — — — — — — — — — — RGB — — — — — — — — — — — — —
*/ 

struct RGB // можно использовать вектор, но...
{
    double r;
    double g;
    double b;

    RGB(double r = 0, double g = 0, double b = 0)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

/*  
— — — — — — — — — — — — RAY — — — — — — — — — — — — —
*/ 

struct Ray
{
    Vector3 direction;
    Vector3 position;
    RGB luminance;

    RGB trace(const Obj & scene) const;
};

RGB Ray::trace(const Obj & scene) const
{
    const vector<Face *> & f = scene.f;
    double len;
    double mostlen = CLIP;
    RGB value = RGB(0, 0, 0);

    for (unsigned long i = 0; i < f.size(); ++i)
    {
        Vector3 & normal = *f[i]->normal[0]; // нужно вычислить относительно точек треугольника
        double dn = direction * normal;

            if (dn > PRECISION || dn < -PRECISION) // проверка на параллельность
        {
            Vector3 & a = f[i]->vertex[0]->position;
            Vector3 & b = f[i]->vertex[1]->position;
            Vector3 & c = f[i]->vertex[2]->position;

            Vector3 &an = *f[i]->normal[0];
            Vector3 &bn = *f[i]->normal[1];
            Vector3 &cn = *f[i]->normal[2];

            len = (f[i]->vertex[0]->position * normal - position * normal) / (dn);
            Vector3 point = position + direction * len;

            double cw = triangleArea(point, a, b) / f[i]->s; //Барицентрические координаты
            double aw = triangleArea(point, b, c) / f[i]->s;
            double bw = triangleArea(point, c, a) / f[i]->s;

                if (aw + bw + cw <= 1 + PRECISION) // Проверка на принадлежность треугольнику
            {
                Vector3 pn = an * aw + bn * bw + cn * cw; // Нормаль в точке попадания луча
                if (len < mostlen)
                {
                    mostlen = len;
                    value = RGB(len, len, len);
                }
            }
        }
        
    }
    return value;
}

/*  
— — — — — — — — — — CAMERA SENSOR — — — — — — — — — —
*/  

struct Sensor
{
    vector< vector<RGB> > value;
    Vector3 origin;
    double scale;
    int resx;
    int resy;
    Sensor(int resx, int resy, double s, double d, Vector3 campos);

    void toTXT(string filename);
};

Sensor::Sensor(int resx, int resy, double s, double d, Vector3 campos)
{
    this->resx = resx;
    this->resy = resy;
    
    scale = s;

    origin.x = - d;
    origin.y = - resx * scale / 2;
    origin.z = resy * scale / 2;

    value.resize(resy);
    for (int i = 0; i < resy; ++i)
        value[i].resize(resx);
}

void Sensor::toTXT(string filename)
{
    ofstream file(filename);

    file << std::scientific;

    if (file.is_open()) 
    {
        for (int y = 0; y < resy; ++y)
        {
            for (int x = 0; x< resx; ++x)
                file << "   " << value[y][x].r;
            file << '\n';
        }

        file << ' ' << '\n';

        for (int y = 0; y < resy; ++y)
        {
            for (int x = 0; x< resx; ++x)
                file << "   " << value[y][x].g;
            file << '\n';
        }
        
        file << '\n';

        for (int y = 0; y < resy; ++y)
        {
            for (int x = 0; x< resx; ++x)
                file << "   " << value[y][x].b;
            file << '\n';
        }
    }
    else
        throw "Incorrect file path!";
    file.close();
}

/*  
— — — — — — — — — — — 3D CAMERA — — — — — — — — — — —
*/  

struct Camera   
{
    Vector3 position;
    Vector3 direction;
    Vector3 rotation;
    Sensor * sensor;

    Camera(Vector3 pos, Vector3 rot, int resx, int resy, double s, double d)
    {
        position = pos;
        direction = Vector3 (CAM_DIR);
        rotation = rot;
        sensor = new Sensor(resx, resy, s, d, pos);
    }

    void render (const Obj & scene);
};

void Camera::render(const Obj & scene)
{

    double scale = sensor->scale;
    Vector3 & origin = sensor->origin;
    Vector3 offset;

    for (int y = 0; y < sensor->resy; ++y)
    {
        offset.z = - ( y * scale + scale / 2 );
        for (int x = 0; x < sensor->resx; ++x)
        {
            offset.y = x * scale + scale / 2; 

            Ray ray;
            ray.direction = (origin + offset).normalized();
            ray.position = position;

            sensor->value[y][x] = ray.trace(scene);
        }
    }
    cout << "DONE!";
}

/*  
— — — — — — — — — — — — MAIN — — — — — — — — — — — —
*/  
    
int main()
{
    Obj o(OBJF);

    Vector3 pos(CAM_POS);
    Vector3 rot(CAM_ROT);

    Camera cam(pos, rot, SNS_RESX, SNS_RESY, SNS_SCALE, SNS_DIST);

    cam.render(o);
    cam.sensor->toTXT(OUTPUT);
}