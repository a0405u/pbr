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

#include "PointCloud.hpp"

#define _USE_MATH_DEFINES

#define FILENAME "Import/Room/Room.shp"
#define OUTPUT "Render/OUT.ppm"

// #define CAM_POS          6, 0, 2.5813
#define CAM_POS             10, -0.21801, 2.5858
#define CAM_ROT             0, 0, 0
#define CAM_CLIP            100

#define SNS_RESX            480
#define SNS_RESY            480
#define SNS_SCALE           0.003
#define SNS_DIST            2

#define PRECISION           0.0001
#define RAYS_PER_PIXEL      99
#define THREADS             4

#define CLOUD_RESOLUTION    256
#define CLOUD_OUTPUT        "Export/OUT.ply"

using namespace std;

Scene scene;

int exec()
{
    string command;
    cin >> command;

    if (command == "exit")
        return 0;
    
    if (command == "debug")
    {
        scene = Scene(FILENAME);

        // s.addLight(new PointLight(200, Vector3(4, 2, 6), RGB(0.3, 0.7, 1)));
        // s.addLight(new PointLight(200, Vector3(-2, -5, -4), RGB(1, 0, 0.4)));
        // s.addLight(new PointLight(200, Vector3(-3, 0, 3.6), RGB(1, 1, 1)));
        scene.addLight(new PlaneLight(100, Vector3(-3.567967, -0.884011, 5.3), 1.05, 1.3));

        scene.addCamera(new Camera(Vector3(CAM_POS), Vector3(CAM_ROT), SNS_RESX, SNS_RESY, SNS_SCALE, SNS_DIST));

        //scene.selectCamera(0);
        //scene.renderThreaded(RAYS_PER_PIXEL, THREADS);
        //scene.mainCamera->sensor->toPPM(OUTPUT);

        PointCloud pc = PointCloud(scene, CLOUD_RESOLUTION);
        pc.exportPLY(CLOUD_OUTPUT);
    }

    if (command == "load")
    {
        string filepath;
        cin >> filepath;
        scene = Scene(filepath);
    }

    if (command == "add")
    {
        string entity;
        cin >> entity;
        if (entity == "light")
        {
            string type;
            cin >> type;
            if (type == "point")
            {
                double i, x, y, z, r, g, b;
                cin >> i >> x >> y >> z >> r >> g >> b;
                scene.addLight(new PointLight(i, Vector3(x, y, z), RGB(r, g, b)));
            }
            if (type == "plane")
            {
                double i, x, y, z, sx, sy, rx, ry, rz, r, g, b;
                cin >> x >> y >> z >> sx >> sy >> rx >> ry >> rz >> r >> g >> b;
                scene.addLight(new PlaneLight(i, Vector3(x, y, z), sx, sy, Vector3(rx, ry, rz), RGB(r, g, b)));
            }
        }
        if (entity == "camera")
        {
            double x, y, z, dx, dy, dz, rx, ry, s, d;
            cin >> x >> y >> z >> dx >> dy >> dz >> rx >> ry >> s >> d;
            scene.addCamera(new Camera(Vector3(x, y, z), Vector3(dx, dy, dz), rx, ry, s, d));
        }
    }

    if (command == "render")
    {
        int r, t;
        cin >> r, t;
        scene.renderThreaded(r, t);
    }

    if (command == "cloud")
    {
        int resolution;
        string filepath;
        cin >> resolution >> filepath;
        PointCloud(scene, resolution).exportPLY(filepath);
    }

    return 1;
}
    
int main(int argc, char *argv[])
{
    srand(time(nullptr));

    string filename = FILENAME;

    if (argc > 1 and strcmp(argv[1], "debug") == 0)
    {
        scene = Scene(FILENAME);
        PointCloud(scene, CLOUD_RESOLUTION).exportPLY(CLOUD_OUTPUT);
        return 0;
    }

    int state = 1;
    cout << "ВВЕДИТЕ КОМАНДУ:\n";

    while (state != 0)
    {
        state = exec();
    }
}