#include "Sensor.hpp"

#define GAMMA   2.2
#define PPM_MAXVAL   255

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

        file << '\n';

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

void Sensor::toPPM(string filename)
{
    ofstream file(filename);

    if (file.is_open()) 
    {
        file << "P3" << '\n';
        file << resx << ' ' << resy << '\n';
        file << PPM_MAXVAL << '\n';

        double max = 0;
        double norm = 1;

        for (int y = 0; y < resy; ++y)
        {
            for (int x = 0; x< resx; ++x)
            {
                if (value[y][x].r > max)
                    max = value[y][x].r;

                if (value[y][x].g > max)
                    max = value[y][x].g;
                
                if (value[y][x].b > max)
                    max = value[y][x].b;
            }
        }

        if (max > 0)
            norm = 1 / max;

        for (int y = 0; y < resy; ++y)
        {
            for (int x = 0; x< resx; ++x)
            {
                file << setw( 3 ) << (int)(pow(value[y][x].r * norm, 1.0 / GAMMA) * PPM_MAXVAL) << ' ';
                file << setw( 3 ) << (int)(pow(value[y][x].g * norm, 1.0 / GAMMA) * PPM_MAXVAL) << ' ';
                file << setw( 3 ) << (int)(pow(value[y][x].b * norm, 1.0 / GAMMA) * PPM_MAXVAL) << '\n';
            }
        }
    }
    else
        throw "Incorrect file path!";
    file.close();
}