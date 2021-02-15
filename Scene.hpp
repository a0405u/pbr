#ifndef SCENE_H
#define SCENE_H

#include "Geometry.hpp"
#include "PolygonalGeometry.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Lambert.hpp"
#include "Emissive.hpp"

using namespace std;

struct Scene
{
    vector< Geometry * > geometry; // Геометрия сцены
    vector< Light * > light; // Свет сцены
    vector< Camera * > camera; // Камеры сцены
    vector< Material * > material; // Материалы сцены

    Camera * mainCamera; // Указатель на выбранную в качестве основной камеру

    Scene();
    Scene(Geometry * g); // Конструктор сцены с геометрией g
    Scene(string filename); // Конструктор сцены из файла

    void addGeometry(Geometry * g);
    void addLight(Light * l); // Добавление источника света l в сцену
    void addCamera(Camera * c); // Добавление камеры c в сцену
    void selectCamera(int n); // Выбор камеры под номером n в качестве основной
    Point * trace(Ray & r) const;
    void render(int n);

};

#endif