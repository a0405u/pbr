#include "Scene.hpp"

Scene::Scene(): mainCamera(NULL)
{
    return;
}

Scene::Scene(string filename): mainCamera(NULL)
{
    if (filename.substr(filename.find_last_of(".") + 1) == "obj")
    {
        loadObj(filename);
    }
    else if (filename.substr(filename.find_last_of(".") + 1) == "shp")
    {
        loadShp(filename);
    }
}

void Scene::loadObj(string filename)
{
    ifstream file(filename);

    if (file.is_open()) 
    {
        PolygonalGeometry * object = new PolygonalGeometry(); // Создаём пустой объект геометрии
        addGeometry(object); // Добавляем его в массив сцены
        string line;

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
                        if (value.size() > 1)
                            // texture.push_back(v[stoi(value[1]) - 1]); координаты текстур
                        if (value.size() > 2)
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
                            object->addMaterial(material[i]);
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

void Scene::loadShp(string filename)
{
    ifstream file(filename);

    if (file.is_open()) 
    {
        PolygonalGeometry * object = new PolygonalGeometry(); // Создаём пустой объект геометрии
        addGeometry(object); // Добавляем его в массив сцены
        string line;

        while(!file.eof())
        {
            getline(file, line);

            if (line.substr(0, 8) == "vertices")
            {
                do
                {
                    getline(file, line);
                    object->addVertex(new Vertex(line));
                }
                while(line.substr(0, 9) != "triangles" and !file.eof());
            }
            if (line.substr(0, 9) == "triangles")
            {
                do
                {
                    getline(file, line);

                    Face * f = new Face();

                    istringstream ss(line);
                    unsigned int a, b, c;

                    ss >> a >> b >> c;
                    
                    f->addVertex(object->vertex[a]);
                    f->addVertex(object->vertex[b]);
                    f->addVertex(object->vertex[c]);

                    Vector3 normal = f->getNormal();

                    f->addVertexNormal(new Vector3(normal));
                    f->addVertexNormal(new Vector3(normal));
                    f->addVertexNormal(new Vector3(normal));

                    f->getArea();

                    if (isnan(f->area))
                        continue;

                    object->addFace(f);
                }
                while(line.substr(0, 5) != "parts" and !file.eof());
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

    if (not mainCamera)
        mainCamera = c; 
}

void Scene::clear()
{
    for (Geometry * g : geometry)
        delete g;
    geometry.clear();

    for (Material * m : material)
        delete m;
    material.clear();

    for (Light * l : light)
        delete l;
    light.clear();

    for (Camera * c : camera)
        delete c;
    camera.clear();
}

void Scene::clearGeometry()
{
    for (Geometry * g : geometry)
        delete g;
    geometry.clear();
}

void Scene::selectCamera(int n)
{
    mainCamera = camera[n];
}

Point * Scene::trace(Ray & ray) const
{
    Point * t = NULL, * p = NULL;

    for (Light * l : light)
    {
        t = l->trace(ray);
        if (t) p = t;
    }
    for (Geometry * g : geometry)
    {
        t = g->trace(ray);
        if (t) p = t;
    }
    ray.count--;
    return p;
}

Point * Scene::traceGeometry(Ray & ray) const
{
    Point * t = NULL, * p = NULL;

    for (Geometry * g : geometry)
    {
        t = g->trace(ray);

        if (t) p = t;
    }
    ray.count--;
    return p;
}

void Scene::renderThread(unsigned int passes, unsigned int threads, unsigned int timelimit, chrono::steady_clock::time_point begin, unsigned int * done)
{
    double scale = mainCamera->sensor->scale;
    Vector3<double> & origin = mainCamera->sensor->origin;
    Vector3 offset;
    chrono::steady_clock::time_point current;
    
    for (unsigned int i = 0; i < passes; i++)
    {
        for (unsigned int y = 0; y < mainCamera->sensor->resy; ++y)
            for (unsigned int x = 0; x < mainCamera->sensor->resx; ++x)
            {
                offset.y = scale * ( x + (double)rand() / RAND_MAX ); 
                offset.z = - scale * ( y + (double)rand() / RAND_MAX );

                Ray ray = Ray((origin + offset).normalized(), mainCamera->position); // Генерация луча

                while (ray.count > 0 and ray.color != 0)
                {
                    Point * p = trace(ray); // Находим точку пересечения луча и геометрии

                    if (p) // Если точка существует
                        mainCamera->sensor->value[y][x] += p->material->luminance(*p, ray, *this);
                    else
                        break;
                }
            }
        (*done)++;
        current = chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(current - begin).count() > timelimit)
            return;
        cout << '\r' << "RENDERING " << std::chrono::duration_cast<std::chrono::seconds>(current - begin).count() << " SECONDS " << (*done) * 100 / (passes * threads) << "% DONE" << flush;
    }
}

string Scene::renderThreaded(unsigned int passes, unsigned int threads, unsigned int timelimit)
{
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    vector<thread *> vt;

    unsigned int done = 0;

    cout << '\r' << "RENDERING 0%" << flush;

    for (unsigned int i = 0; i < threads; i++)
    {
        vt.push_back(new thread(&Scene::renderThread, this, passes/threads, threads, timelimit, begin, &done));
    }
    for (unsigned int i = 0; i < threads; i++)
    {
        vt[i]->join();
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << endl << "FINISHED IN " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << " SECONDS WITH " << done << " PASSES" << endl;
    return to_string(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count());
}