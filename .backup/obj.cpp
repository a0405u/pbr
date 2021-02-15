#include "obj.hpp"

using namespace std;

Obj::Obj(string filename)
{
    ifstream fs(filename);
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
                            break;
                        default:
                            break;
                    }
                    break;
                case 'f':
                    f.push_back(new Face(line.substr(2), v));
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