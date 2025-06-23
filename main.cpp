#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>

#include "Constants.hpp"
#include "Headers.hpp"
#include "Obj.hpp"
#include "Vector3.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "PolygonalGeometry.hpp"
#include "PointGeometry.hpp"
#include "VoxelGeometry.hpp"
#include "VoxelGeometryA.hpp"
#include "PointLight.hpp"
#include "PlaneLight.hpp"
#include "triangleArea.hpp"

#define _USE_MATH_DEFINES

// #define FILENAME "Import/Room/Room.shp"
// #define FILENAME "Scenes/CornellBoxNoLight.obj"
// #define FILENAME "Scenes/CornellBoxRotated.obj"
// #define FILENAME "Scenes/Bunny.obj"
#define FILENAME "Scenes/Bunnys.obj"
#define OUTPUTPPM "Render/OUT.ppm"
#define OUTPUTTXT "Render/OUT.txt"
#define OUTPUTDIR "Render/"

// #define CAM_POS          6, 0, 2.5813
#define CAM_POS             10, -0.21801, 2.5858
#define CAM_ROT             0, 0, 0
#define CAM_CLIP            100

#define LIGHT_POS           -3.567967, -0.884011, 5.29
#define LIGHT_SIZE          1.05, 1.3

#define SNS_RESX            480
#define SNS_RESY            480
#define SNS_SCALE           0.003
#define SNS_DIST            2

#define RAYS_PER_PIXEL      32 //128
#define TIMELIMIT           99999
#define THREADS             4

#define CLOUD_RESOLUTION    24000
#define CLOUD_OUTPUT        "Export/CLOUDOUT.ply"

#define VOXEL_RESOLUTION    256
#define VOXEL_OUTPUT        "Export/VOXELOUT.ply"

using namespace std;

Scene scene;

int exec()
{
    string command;
    cin >> command;

    if (command == "exit")
        return 0;
    
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
                cin >> i >> x >> y >> z >> sx >> sy >> rx >> ry >> rz >> r >> g >> b;
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
        PointGeometry(scene, resolution).exportPLY(filepath);
    }

    return 1;
}
    
int main(int argc, char *argv[])
{
    srand(time(nullptr));

    string filename = FILENAME;
    string outputfilename = to_string(RAYS_PER_PIXEL) + "p_" + to_string(VOXEL_RESOLUTION) + "v_";

    if (argc > 1 and strcmp(argv[1], "debug") == 0)
    {
        scene = Scene(FILENAME);

        PointGeometry * pg = new PointGeometry(scene, CLOUD_RESOLUTION);
        pg->exportPLY(CLOUD_OUTPUT);

        VoxelGeometryA * vg = new VoxelGeometryA(*pg, VOXEL_RESOLUTION);
        vg->exportPLY(VOXEL_OUTPUT);

        // scene.clear();
        scene.clearGeometry();
        delete pg;

        scene.addGeometry(vg);
        scene.addLight(new PlaneLight(100, Vector3<double>(LIGHT_POS), LIGHT_SIZE));
        scene.addCamera(new Camera(Vector3<double>(CAM_POS), Vector3<double>(CAM_ROT), SNS_RESX, SNS_RESY, SNS_SCALE, SNS_DIST));

        outputfilename += scene.renderThreaded(RAYS_PER_PIXEL, THREADS, TIMELIMIT) + "s";
        scene.mainCamera->sensor->toPPM(OUTPUTDIR + outputfilename + ".ppm");
        // scene.mainCamera->sensor->toTXT(OUTPUTDIR + outputfilename + ".txt");
        scene.mainCamera->sensor->toCSV(OUTPUTDIR + outputfilename + ".csv");
        return 0;
    }

    int state = 1;
    cout << "ВВЕДИТЕ КОМАНДУ:\n";

    while (state != 0)
    {
        state = exec();
    }
}
