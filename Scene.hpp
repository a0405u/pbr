#ifndef SCENE_H
#define SCENE_H

#include "Geometry.hpp"
#include "PolygonalGeometry.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Lambert.hpp"
#include "Emissive.hpp"

#include <chrono>
#include <thread>
#include <future>

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

    void loadObj(string filename); // Загрузка сцены из файла Obj
    void loadShp(string filename); // Загрузка сцены из файла Shp

    void addGeometry(Geometry * g); // Добавление геометрии g в сцену
    void addLight(Light * l); // Добавление источника света l в сцену
    void addCamera(Camera * c); // Добавление камеры c в сцену

    void clear(); // Очистка сцены
    void clearGeometry();

    void selectCamera(int n); // Выбор камеры под номером n в качестве основной

    Point * trace(Ray & r) const; // Трассировка луча в сцене
    Point * traceGeometry(Ray & r) const; // Трассировка луча в сцене без источников света

    string renderThreaded(unsigned int passes = 2, unsigned int threads = 2, unsigned int timelimit = 60); // Рендер в несколько потоков
    void renderThread(unsigned int passes = 1, unsigned int threads = 1, unsigned int timelimit = 60, chrono::steady_clock::time_point begin = chrono::steady_clock::now(), unsigned int * done = NULL); // Рендер одного потока

    void exportPLY(const string filename, unsigned int resolution) const;
};

#endif