#ifndef SENSOR_H
#define SENSOR_H

#include <fstream>

#include "Vector3.hpp"
#include "RGB.hpp"

using namespace std;

struct Sensor
{
    vector< vector< RGB > > value; // Двумерный массив пикселей матрицы
    Vector3<double> origin; //
    double scale;
    int resx;
    int resy;
    Sensor(int resx, int resy, double s, double d, Vector3<double> campos);

    void toTXT(string filename);
    void toPPM(string filename);
    void toCSV(string filename);
};

#endif