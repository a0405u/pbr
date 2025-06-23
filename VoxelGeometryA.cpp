#include "VoxelGeometryA.hpp"
#include "ComplexMaterial.hpp"
#include "Headers.hpp"

VoxelGeometryA::VoxelGeometryA(const PointGeometry & cloud, unsigned int gridSize)
{
    Vector3<double> min = cloud.point[0]->position;
    Vector3<double> max = cloud.point[0]->position;

    // Определение крайних точек облака
    for (Point * point : cloud.point) 
    {
        if (point->position.x < min.x)
            min.x = point->position.x;
        
        if (point->position.y < min.y)
            min.y = point->position.y;

        if (point->position.z < min.z)
            min.z = point->position.z;

        if (point->position.x > max.x)
            max.x = point->position.x;

        if (point->position.y > max.y)
            max.y = point->position.y;

        if (point->position.z > max.z)
            max.z = point->position.z;
    }

    // Определение размеров и положения воксельной сетки
    position = min;
    end = max + BOUNDARY;
    size = end - position;

    double sizeMax = size.x;

    if (size.y > sizeMax) sizeMax = size.y;
    if (size.z > sizeMax) sizeMax = size.z;

    // Определение размера вокселя в координатах
    scale = sizeMax / gridSize;

    // Определение разрешения воксельной сетки
    resolution = Vector3<unsigned int>(gridSize, gridSize, gridSize);
    count = resolution.x * resolution.y * resolution.z;

    // Инициализация массивов для хранения вокселей
    voxel = new Voxel ** * [resolution.x];

    for (unsigned int x = 0; x < resolution.x; x++)
    {
        voxel[x] = new Voxel * * [resolution.y];

        for (unsigned int y = 0; y < resolution.y; y++)
        {
            voxel[x][y] = new Voxel * [resolution.z];

            for (unsigned int z = 0; z < resolution.z; z++)
            {
                voxel[x][y][z] = nullptr;
            }
        }
    }

    // Создание вокселей из точек

    Vector3<unsigned int> p; // Позиция вокселя

    unsigned int * n = new unsigned int[count]; // Количество точек в вокселе
    unsigned int v;

    for (unsigned int i = 0; i < count; i++) // ЭТО ВСЁ УЖЕ НЕ НУЖНО
        n[i] = 0;

    for (Point * point : cloud.point) // Перебор всех точек
    {
        p = locate(point->position - position); // Обнаруживаем воксель для текущей точки
        v = p.x + p.y * resolution.x + p.z * resolution.x * resolution.y; // Номер вокселя

        if (++n[v] == 1) // Если в этой клетке воксель ещё не создавался
            addVoxel(p, point->normal, point->material); // Создаём воксель
        else
        {
            if (n[v] == 2) // Если создавался, преобразовываем его материал в комплексный
                voxel[p.x][p.y][p.z]->newMaterial(new ComplexMaterial(voxel[p.x][p.y][p.z]->material));
            
            // Так как в этом сегменте кода мы можем оказаться лишь в случае преобразования материала вокселя в комплексный
            dynamic_cast<ComplexMaterial *>(voxel[p.x][p.y][p.z]->material)->addMaterial(point->material); // Добавляем материал
            voxel[p.x][p.y][p.z]->normal += point->normal; // Добавляем нормаль
        }
    }

    for (unsigned int x = 0; x < resolution.x; x++)
        for (unsigned int y = 0; y < resolution.y; y++)
            for (unsigned int z = 0; z < resolution.z; z++)
            {
                v = x + y * resolution.x + z * resolution.x * resolution.y; // Номер вокселя

                if (n[v] > 0)
                    voxel[x][y][z]->normal.normalize();
            }
}

VoxelGeometryA::~VoxelGeometryA()
{
    for (unsigned int x = 0; x < resolution.x; x++)
        for (unsigned int y = 0; y < resolution.y; y++)
            for (unsigned int z = 0; z < resolution.z; z++)
                delete voxel[x][y][z];
}

Voxel * VoxelGeometryA::addVoxel(const Vector3<unsigned int> p)
{
    voxel[p.x][p.y][p.z] = new Voxel(); // Создаём пустой воксель
    return voxel[p.x][p.y][p.z];
}

Voxel * VoxelGeometryA::addVoxel(const Vector3<unsigned int> p, const Vector3<double> n, const Material * m)
{
    voxel[p.x][p.y][p.z] = new Voxel(n, m->clone()); // Создаём новый воксель
    return voxel[p.x][p.y][p.z];
}

Voxel * VoxelGeometryA::addVoxel(const Vector3<double> p, const Vector3<double> n, const Material * m)
{
    Vector3<unsigned int> vp = locate(p);
    voxel[vp.x][vp.y][vp.z] = new Voxel(n, m->clone()); // Создаём новый воксель
    return voxel[vp.x][vp.y][vp.z];
}

Voxel * VoxelGeometryA::getVoxel(const Vector3<unsigned int> & p) const
{
    return voxel[p.x][p.y][p.z];
}

Vector3<unsigned int> VoxelGeometryA::locate(const Vector3<double> & p) const
{
    return Vector3<unsigned int>(p.x / scale, p.y / scale, p.z / scale);
}

Vector3<double> VoxelGeometryA::getPosition(const Vector3<unsigned int> & p) const
{
    return Vector3<double>(p.x * scale, p.y * scale, p.z * scale);
}

void VoxelGeometryA::exportPLY(const string filename) const
{
    ofstream file(filename);

    if (file.is_open()) 
    {
        unsigned int voxels = 0;

        for (unsigned int x = 0; x < resolution.x; x++)
            for (unsigned int y = 0; y < resolution.y; y++)
                for (unsigned int z = 0; z < resolution.z; z++)
                    if (voxel[x][y][z])
                        voxels++;

        file << "ply" << '\n';
        file << "format ascii 1.0" << '\n';
        file << "element vertex " << voxels << '\n';
        file << "property float x" << '\n';
        file << "property float y" << '\n';
        file << "property float z" << '\n';
        file << "property float nx" << '\n';
        file << "property float ny" << '\n';
        file << "property float nz" << '\n';
        file << "property uchar red" << '\n';
        file << "property uchar green" << '\n';
        file << "property uchar blue" << '\n';
        file << "end_header" << '\n';

        RGB color;
        Vector3<double> normal;

        for (unsigned int x = 0; x < resolution.x; x++)
            for (unsigned int y = 0; y < resolution.y; y++)
                for (unsigned int z = 0; z < resolution.z; z++)
                    if (voxel[x][y][z])
                    {
                        color = voxel[x][y][z]->material->getDiffuse() * 255;
                        normal = voxel[x][y][z]->normal; 

                        file << (float)(position.x + (scale * x)) << ' ' << (float)(position.y + (scale * y)) << ' ' << (float)(position.z + (scale * z)) << ' ';
                        file << (float)normal.x << ' ' << (float)normal.y << ' ' << (float)normal.z << ' ';
                        file << (int)color.r << ' ' << (int)color.g << ' ' << (int)color.b << '\n';
                    }
    }
    else
        throw "Incorrect file path!";
    file.close();
}


bool VoxelGeometryA::inside(const Vector3<double> & p) const
{
    if ( p.x < position.x or p.x >= end.x or p.y < position.y or p.y >= end.y or p.z < position.z or p.z >= end.z )
        return false;

    return true;
}


double VoxelGeometryA::intersect(const Ray & ray) const
{
    Vector3< double > bound = position + BOUNDARY; // Проверяемые границы бокса
    Vector3< double > point; // Точка пересечения
    Vector3< double > t; // Расстояние до пересечения с плоскостями

    for (unsigned int i = 0; i < 3; i++) // По трём осям координат
    {
        // Определяем плоскости пересечения
        if (ray.direction[i] < 0) bound[i] = end[i] - BOUNDARY;

        // Находим расстояния до пересечения
        t[i] = (bound[i] - ray.position[i]) / ray.direction[i];

        // Находим координаты точек пересечения
        point = ray.position + ray.direction * t[i];

        // Определяем принадлежность пересечения к боксу
        if (point[i + 1 % 3] < position[i + 1 % 3] + BOUNDARY)
            t[i] = -1;
        if (point[i + 1 % 3] >= end[i + 1 % 3] - BOUNDARY)
            t[i] = -1;
        if (point[i + 2 % 3] < position[i + 2 % 3] + BOUNDARY)
            t[i] = -1;
        if (point[i + 2 % 3] >= end[i + 2 % 3] - BOUNDARY)
            t[i] = -1;
    }
    
    // Определяем плоскость пересечения

    if (t.x >= 0) return t.x;
    if (t.y >= 0) return t.y;
    if (t.z >= 0) return t.z;

    return -1;
}


Point * VoxelGeometryA::trace(Ray & ray) const
{
    double length = 0;
    Vector3<double> relativePosition = ray.position;

    if (inside(ray.position)) // Если луч находится внутри сетки
    {
        relativePosition = ray.position - position; // Находим его позицию относительно позиции сетки
    }
    else // Если луч вне сетки, ищем точку пересечения луча с областью сетки
    {
        length = intersect(ray);

        if (length < 0) // Точка пересечения не нашлась
            return NULL;

        relativePosition = ray.position + ray.direction * length - position;
    }

    // Обработка края сетки

    if (relativePosition.x >= size.x)
        relativePosition.x -= BOUNDARY;

    if (relativePosition.y >= size.y)
        relativePosition.y -= BOUNDARY;

    if (relativePosition.z >= size.z)
        relativePosition.z -= BOUNDARY;

    // Обнаруживаем воксель для начала трассировки
    Vector3<unsigned int> currentVoxelId = locate(relativePosition);
    Voxel * currentVoxel = getVoxel(currentVoxelId);

    if (not currentVoxel or length == 0) // Если текущий воксель не является пересечением
    {
        // Направление в котором будем перемещаться по вокселям (1; -1; 0)
        Vector3<int> direction = Vector3<int>(sign(ray.direction.x), sign(ray.direction.y), sign(ray.direction.z));

        // Граница для поиска пересечения луча
        Vector3<double> bound = getPosition(currentVoxelId);

        if (direction.x > 0)
            bound.x += scale;
        if (direction.y > 0)
            bound.y += scale;
        if (direction.z > 0)
            bound.z += scale;

        // Расстояние до пересечения с вокселями соответствующих направлений
        Vector3<double> maxLength = Vector3<double>
        (
            (bound.x - relativePosition.x) / ray.direction.x * direction.x, 
            (bound.y - relativePosition.y) / ray.direction.y * direction.y, 
            (bound.z - relativePosition.z) / ray.direction.z * direction.z
        );

        // Расстояние для преодоления целого вокселя в направлении
        Vector3<double> deltaLength = Vector3<double>
        (
            scale / ray.direction.x * direction.x,
            scale / ray.direction.y * direction.y,
            scale / ray.direction.z * direction.z
        );

        // Текущая ось координат: 0 - Х, 1 - Y, 2 - Z
        int current;

        while (true) // Цикл прохода по сетке вокселей
        {
            current = 0;

            if (maxLength.y < maxLength[current])
                current = 1;
            if (maxLength.z < maxLength[current])
                current = 2;

            // Переход к следующему вокселю по оси координат
            currentVoxelId[current] += direction[current];

            // Выход за пределы сетки, конец трассировки
            if (currentVoxelId[current] >= resolution[current])
                return NULL;

            // Получение ссылки на текущий воксель
            currentVoxel = getVoxel(currentVoxelId);

            if (currentVoxel) // Если он не пустой
            {
                length += maxLength[current];
                break; // Найден воксель пересечения
            }

            maxLength[current] += deltaLength[current];
        }
    }

    // Если была найдена точка пересечения луча и геометрии
    if(length < ray.length) // И она ближе предыдущей
    {
        ray.length = length - PRECISION; // Сохраняем в луч найденное расстояние
        return new Point(ray.position + ray.direction * ray.length, currentVoxel->normal, currentVoxel->material);
    }
    return NULL;
}
