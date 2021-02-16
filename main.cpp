#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>

#include "Headers.hpp"
#include "Obj.hpp"
#include "Vector3.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "PolygonalGeometry.hpp"
#include "PointLight.hpp"
#include "PlaneLight.hpp"
#include "triangleArea.hpp"

#define _USE_MATH_DEFINES

#define OBJF "Scenes/CornellBoxRotated.obj"
#define OUTPUT "Render/OUT.ppm"

// #define CAM_POS      6, 0, 2.5813
#define CAM_POS      10, -0.21801, 2.5858
#define CAM_ROT      0, 0, 0
#define CAM_CLIP     100

#define SNS_RESX     480
#define SNS_RESY     480
#define SNS_SCALE    0.003
#define SNS_DIST     2

#define PRECISION 0.0001
#define RAYS_PER_PIXEL 99
#define THREADS 4

using namespace std;


void exec(string command)
{
    if (command == "exit")
        return;
}
    
int main(int argc, char *argv[])
{
    srand(time(nullptr));

    string filename = OBJF;
    // string command = "";

    // cout << "ВВЕДИТЕ КОМАНДУ:\n";

    // while (command != "exit")
    // {
    //     cin >> command;
    //     exec(command);
    // }

    Scene scene = Scene(OBJF);

    // s.addLight(new PointLight(200, Vector3(4, 2, 6), RGB(0.3, 0.7, 1)));
    // s.addLight(new PointLight(200, Vector3(-2, -5, -4), RGB(1, 0, 0.4)));
    // s.addLight(new PointLight(200, Vector3(-3, 0, 3.6), RGB(1, 1, 1)));
    scene.addLight(new PlaneLight(100, Vector3(-3.567967, -0.884011, 5.3), 1.05, 1.3));

    scene.addCamera(new Camera(Vector3(CAM_POS), Vector3(CAM_ROT), SNS_RESX, SNS_RESY, SNS_SCALE, SNS_DIST));

    scene.selectCamera(0);

    scene.renderThreaded(RAYS_PER_PIXEL, THREADS);
    
    scene.mainCamera->sensor->toPPM(OUTPUT);
}