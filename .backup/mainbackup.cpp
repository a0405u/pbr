#include <iostream>
#include <math.h>
#include <vector>

#include "obj.hpp"

#define OBJF "Scenes/Cube.obj"
#define CAM_POS       0, 2, 0
#define CAM_DIR       0, 0, -1
#define CAM_ROT       0, 0, 0

#define SNS_RESX     800
#define SNS_RESY     600
#define SNS_SCALE    0.001
#define SNS_DIST     0.5

using namespace std;

/*  
— — — — — — — — — — — 2D VECTOR — — — — — — — — — — —
*/  

template <typename Ttype> struct Vector2
{
        Ttype x, y;

        Vector2();
        Vector2(Ttype a);
        Vector2(Ttype x, Ttype y);

        Vector2<Ttype> operator*(Ttype m);
};

template <typename Ttype>
Vector2<Ttype>::Vector2()
{
    x = 0; y = 0;
}

template <typename Ttype>
Vector2<Ttype>::Vector2(Ttype a)
{
    x = a; y = a;
}

template <typename Ttype>
Vector2<Ttype>::Vector2(Ttype x, Ttype y)
{
    this->x = x; this->y = y;
}

template <typename Ttype>
Vector2<Ttype> Vector2<Ttype>::operator *(Ttype m)
{
    Vector2<Ttype> v2(x * m, y * m);

    return v2;
}

/*  
— — — — — — — — — — — 3D VECTOR — — — — — — — — — — —
*/  

template <typename Ttype> struct Vector3
{
        Ttype x, y ,z;

        Vector3();
        Vector3(Ttype a);
        Vector3(Ttype x, Ttype y, Ttype z);

        Vector3<Ttype> operator * ( Ttype m) const;
        Vector3<Ttype> operator / (Ttype m) const;
        template <typename Ctype> Vector3<Ttype> operator + (const Vector3<Ctype> v) const;
        template <typename Ctype> Vector3<Ttype> operator + (const Vector2<Ctype> v) const;

        Ttype length() const;
        Vector3<Ttype> normalized() const;
};

template <typename Ttype> 
Vector3<Ttype>::Vector3()
{
    x = 0; y = 0; z = 0;
}

template <typename Ttype> 
Vector3<Ttype>::Vector3(Ttype a)
{
    x = a; y = a; z = a;
}

template <typename Ttype> 
Vector3<Ttype>::Vector3(Ttype x, Ttype y, Ttype z)
{
    this->x = x; this->y = y; this->z = z;
}

template <typename Ttype> 
Vector3<Ttype> Vector3<Ttype>::operator * (Ttype m) const
{
    Vector3<Ttype> v3(x * m, y * m, z * m); 

    return v3;
}

template <typename Ttype> 
Vector3<Ttype> Vector3<Ttype>::operator / (Ttype m) const
{
    Vector3<Ttype> v3(x / m, y / m, z / m); 

    return v3;
}

template <typename Ttype> 
template <typename Ctype> 
Vector3<Ttype> Vector3<Ttype>::operator + (const Vector3<Ctype> v3) const
{
    Vector3<Ttype> newVector3(x + v3.x, y + v3.y, z + v3.z); 

    return newVector3;
}

template <typename Ttype> 
template <typename Ctype> 
Vector3<Ttype> Vector3<Ttype>::operator +(const Vector2<Ctype> v2) const
{
    Vector3<Ttype> newVector3(x + v2.x, y + v2.y, z); 

    return newVector3;
}

template <typename Ttype> 
Ttype Vector3<Ttype>::length() const
{
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

template <typename Ttype> 
Vector3<Ttype> Vector3<Ttype>::normalized() const
{
    Vector3<Ttype> newVector = *this / this->length();
    return newVector;
}

/*  
— — — — — — — — — — — — RGB — — — — — — — — — — — — —
*/ 

struct RGB
{
    double r;
    double g;
    double b;

    RGB()
    {
        r = 0; g = 0; b = 0;
    }
};

/*  
— — — — — — — — — — — — RAY — — — — — — — — — — — — —
*/ 

struct Ray
{
    Vector3<double> direction;
    Vector3<double> position;
    RGB luminance;

    RGB trace(const Obj & scene) const;
};

RGB Ray::trace(const Obj & scene) const
{
    RGB val;
    return val;
}

/*  
— — — — — — — — — — CAMERA SENSOR — — — — — — — — — —
*/  

struct Sensor
{
    vector< vector<RGB> > value;
    Vector3<double> origin;
    int resx;
    int resy;

    Sensor(int resx, int resy, double s, double d)
    {
        this->resx = resx;
        this->resy = resy;

        origin.x = -resx * s;
        origin.y = -resy * s;
        origin.z = -d;

        value.resize(resy);
        for (int i = 0; i < resy; ++i)
            value[i].resize(resx);
    }
};

/*  
— — — — — — — — — — — 3D CAMERA — — — — — — — — — — —
*/  

struct Camera
{
    Vector3<double> position;
    Vector3<double> direction;
    Vector3<double> rotation;
    Sensor * sensor;

    Camera(Vector3<double> pos, Vector3<double> rot, int resx, int resy, double s, double d)
    {
        position = pos;
        direction = Vector3<double> (CAM_DIR);
        rotation = rot;
        sensor = new Sensor(resx, resy, s, d);
    }

    void render (const Obj & scene);

};

void Camera::render(const Obj & scene)
{

    Vector3<double> & origin = sensor->origin;

    Vector3<double> offset;

    Ray ray;

    for (int y = 0; y < sensor->resy; ++y)
        for (int x = 0; x < sensor->resx; ++x)
        {
            offset.x = x; offset.y = y;

            ray.direction = (origin + offset).normalized();
            ray.position = position;

            sensor->value[y][x] = ray.trace(scene);
            
        }


}
    
int main()
{
    Obj o(OBJF);

    Vector3<double> pos(CAM_POS);
    Vector3<double> rot(CAM_ROT);

    Camera cam(pos, rot, SNS_RESX, SNS_RESY, SNS_SCALE, SNS_DIST);

    cam.render(o);
}