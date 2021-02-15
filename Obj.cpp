#include "Obj.hpp"

Obj::Obj(string filename)
{
    ifstream fs(filename);
    material = RGB(1, 1, 1);
    string line;
    s = 0;

    if (fs.is_open()) 
    {
        while(!fs.eof())
        {
            getline(fs, line);
            switch(line[0])
            {
                case 'v':
                    switch (line[1])
                    {
                        case ' ': 
                            v.push_back(new Vertex(line.substr(2)));
                            break;
                        case 't':
                            break;
                        case 'n':
                            vn.push_back(new Vector3(line.substr(3)));
                            break;
                        default:
                            break;
                    }
                    break;
                case 'f':
                    f.push_back(new Face(line.substr(2), v, vn));
                    break;
                case 's':
                    if (line == "s on")
                        s = 1;
                    break;
                default:
                    break;
            }
        }
    }
    else
        throw "Incorrect file path!";
    fs.close();
}