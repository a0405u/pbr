#include "Scene.hpp"

Scene::Scene()
{
    return;
}

Scene::Scene(string filename)
{
    ifstream file(filename);
    string line;

    if (file.is_open()) 
    {
        PolygonalGeometry * object = new PolygonalGeometry(); // Создаём пустой объект геометрии
        addGeometry(object); // Добавляем его в массив сцены

        int shiftFace = 0;
        int shiftVertex = 0;
        int shiftVertexNormal = 0;

        while(!file.eof())
        {
            getline(file, line);

            if (line.substr(0, 6) == "mtllib")
            {
                string mtlfilepath = filename.substr(0, filename.rfind('/') + 1) + line.substr(7);
                ifstream mtlfile(mtlfilepath);
                string mtlline;

                if (mtlfile.is_open())
                {
                    RGB ka, kd, ks, ke; 
                    string name = "";

                    while(!mtlfile.eof())
                    {
                        getline(mtlfile, mtlline);
                        if (mtlline.substr(0, 6) == "newmtl")
                        {
                            if (name != "")
                            {
                                material.push_back(new Lambert(kd, name));
                            }
                            name = mtlline.substr(7);
                            continue;
                        }
                            
                        if (mtlline.substr(0, 2) == "Kd") 
                        {
                            vector <string> kds = getWords(mtlline.substr(3));
                            kd = RGB(stod(kds[0]), stod(kds[1]), stod(kds[2]));
                        }

                        if (mtlline.substr(0, 2) == "Ke") 
                        {
                            vector <string> kes = getWords(mtlline.substr(3));
                            ke = RGB(stod(kes[0]), stod(kes[1]), stod(kes[2]));
                        }
                    }
                    if (ke.r != 0 or ke.g != 0 or ke.b != 0)
                        material.push_back(new Emissive(ke, name));
                    else
                        material.push_back(new Lambert(kd, name));
                }
                else
                    throw "Incorrect file path!";
                mtlfile.close();
            }

            switch(line[0])
            {
                case 'v': 
                {
                    switch (line[1])
                    {
                        case ' ': // Строка вершины
                        {
                            object->addVertex(new Vertex(line.substr(2)));
                            break;
                        }
                        case 't': // Строка UV карты
                        {
                            break;
                        }
                        case 'n': // Строка нормали в вершине
                        {
                            object->addVertexNormal(new Vector3(line.substr(3)));
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                case 'f': // Строка полигона
                {
                    Face * f = new Face();

                    vector<string> vs = getWords(line.substr(2)); // Получаем строки типа "v/vt/vn" где vt и vn опциональны
                    
                    for (vector<string>::iterator it = vs.begin(); it != vs.end(); ++it) //перебираем все вершины
                    {
                        vector<string> value = getWords(*it, '/');

                        f->addVertex(object->vertex[stoi(value[0]) - 1 - shiftVertex]);
                        // texture.push_back(v[stoi(value[1]) - 1]); координаты текстур
                        f->addVertexNormal(object->vertexNormal[stoi(value[2]) - 1 - shiftVertexNormal]);
                    }
                    f->getArea();
                    object->addFace(f);
                    break;
                }
                case 's': // Строка обозначающая гладкость объекта
                {
                    if (line == "s on" or line == "s 1")
                        object->smooth = true;
                    break;
                }
                case 'u': // Строка обозначающая материал
                {
                    for (unsigned long i = 0; i < material.size(); i++)
                    {
                        if (material[i]->name == line.substr(7))
                            object->material = material[i];
                    }
                    break;
                }
                case 'o': // Строка обозначающая начало нового объекта
                {
                    if (object->empty()) // Если текущий объект пуст, не создаём новый
                        break;

                    shiftVertex += object->sizeVertex();
                    shiftVertexNormal += object->sizeVertexNormal();
                    shiftFace += object->sizeFace();
                    
                    object = new PolygonalGeometry(); // Создаём пустой объект геометрии
                    addGeometry(object); // Добавляем его в массив сцены
                    break;
                }
                default:
                    break;
            }
        }
    }
    else
        throw "Incorrect file path!";
    file.close();
}

void Scene::addGeometry(Geometry * g)
{
    geometry.push_back(g);
}

void Scene::addLight(Light * l)
{
    light.push_back(l);
}

void Scene::addCamera(Camera * c)
{
    camera.push_back(c);
}

void Scene::selectCamera(int n)
{
    mainCamera = camera[n];
}

Point * Scene::trace(Ray & r) const
{
    Point * t = NULL, * p = NULL;

    for (unsigned long i = 0; i < geometry.size(); ++i)
    {
        t = geometry[i]->trace(r);

        if (t)
            p = t;
    }
    return p;
}

void Scene::render(int n) // Брать случайную точку в пикселе, вместо центральной
{
    double scale = mainCamera->sensor->scale;
    Vector3 & origin = mainCamera->sensor->origin;
    Vector3 offset;

    for (int i = 0; i < n; i++)
    {
        cout << '\r' << "RENDERING " << i * 100 / n  << '%' << flush;

        for (int y = 0; y < mainCamera->sensor->resy; ++y)
        {
            offset.z = - scale * ( y + (double)rand() / RAND_MAX );

            for (int x = 0; x < mainCamera->sensor->resx; ++x)
            {
                offset.y = scale * ( x + (double)rand() / RAND_MAX ); 

                Ray ray = Ray((origin + offset).normalized(), mainCamera->position); // Генерация луча

                Point * p = trace(ray); // Находим точку пересечения луча и геометрии

                if (p) // Если точка существует
                {
                    mainCamera->sensor->value[y][x] = p->material->luminance(*p, ray, *this);
                }
            }
        }
    }
    cout << endl << "FINISHED!" << endl;
}