#include "VoxelGeometry.hpp"
#include "ComplexMaterial.hpp"
#include "Headers.hpp"

VoxelGeometry::VoxelGeometry(const PointGeometry & cloud, unsigned int gridResolution)
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
    end = max + BOUNDARY; // Добавляем границу точности
    size = end - position;

    // Проверка и изменение разрешения сетки на соответсвие степени двойки
    uint temp = 1;
    uint depth = 0; 

    while (temp < gridResolution)
    {
        temp *= 2;
        depth++;
    }

    gridResolution = temp;

    // Определение размера вокселя в координатах
    double maxSize = size.max();
    scale = maxSize / gridResolution;

    // Определение разрешения воксельной сетки
    resolution = Vector3<unsigned int>(gridResolution);
    total = resolution.x * resolution.y * resolution.z;

    // Инициализация массивов для хранения вокселей
    Voxel *** * voxel = new Voxel ** * [resolution.x];

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

    unsigned int * n = new unsigned int[total]; // Количество точек в вокселе
    unsigned int v;

    for (unsigned int i = 0; i < total; i++) // ЭТО ВСЁ УЖЕ НЕ НУЖНО
        n[i] = 0;

    for (Point * point : cloud.point) // Перебор всех точек
    {
        p = locate(point->position - position); // Обнаруживаем воксель для текущей точки
        v = getId(p.x, p.y, p.z); // Номер вокселя

        if (++n[v] == 1) // Если в этой клетке воксель ещё не создавался
            voxel[p.x][p.y][p.z] = new Voxel(point->normal, point->material);
        else
        {
            voxel[p.x][p.y][p.z]->addMaterial(point->material); // Добавляем материал
            voxel[p.x][p.y][p.z]->normal += point->normal; // Добавляем нормаль
        }
    }

    // Нормализация нормалей
    for (unsigned int x = 0; x < resolution.x; x++)
        for (unsigned int y = 0; y < resolution.y; y++)
            for (unsigned int z = 0; z < resolution.z; z++)
            {
                if (n[getId(x, y, z)] > 0)
                    voxel[x][y][z]->normal.normalize();
            }

    delete[] n; // Удаляем временный массив количества точек

    // Преобразование в октодерево рекурсивно начиная с корня

    tree = new Octree(depth);
    tree = createVoxel(tree, Vector3<unsigned int>(0, 0, 0), gridResolution, voxel);


    // // Преобразование массива в окто-дерево начиная с ветвей

    // // Массивы для временного хранения указателей на элементы дерева
    // Octree ** tp = new Octree * [total];
    // Octree ** tc = NULL;

    // // Создание первичных элементов-вокселей окто-дерева
    // for (unsigned int x = 0; x < resolution.x; x++)
    //     for (unsigned int y = 0; y < resolution.y; y++)
    //         for (unsigned int z = 0; z < resolution.z; z++)
    //             tp[getId(x, y, z)] = new Octree(voxel[x][y][z]);

    // // Преобразование массива в октодерево начиная с максимального разрешения
    // for (unsigned int i = 2; i < depth; i++)
    // {
    //     unsigned int k = (i - 1) * 2; // Коэффициент разрешения
    //     unsigned int m = k * 4; // Коэффициент количества вокселей

    //     unsigned int parent = 0;
    //     unsigned int child = 0;

    //     Octree ** tc = tp;
    //     tp = new Octree * [total / m];

    //     for (unsigned int x = 0; x < resolution.x; x += k)
    //         for (unsigned int y = 0; y < resolution.y; y += k)
    //             for (unsigned int z = 0; z < resolution.z; z += k)
    //             {
    //                 parent = getId(x, y, z) / m; // Проверить нумерацию
    //                 child = parent * 8;

    //                 tp[parent] = new Octree(new Voxel(voxel[x][y][z]));

    //                 for (unsigned int i = 0; i < 8; i++)
    //                     tp[parent]->child[i] = tc[child + i];

    //             }
    // }


    // Преобразование массива в окто-дерево начиная с корня

    // tree = new Octree(new Voxel(Vector3<double>(0, 0, 0), new ComplexMaterial()));
    // tree->child = new Octree * [8];

    // Octree * parent = tree;
    // Octree * child = NULL;

    // Vector3<unsigned int> segmentPosition = Vector3<unsigned int> (0, 0, 0);
    // Vector3<unsigned int> segmentSize = Vector3<unsigned int> (resolution);
    // Voxel * axisVoxel = NULL;

    // for (unsigned int i = 0; i < depth; i++)
    // {
    //     //Производить рекурсивно, спускаясь в самый конец дерева, и только при обработке всех потомков узла, создавать в нём воксель, это избавит от лишних проходов по вокселям дерева
    //     // СЕГМЕНТОВ ВСЕГДА ВОСЕМЬ И ОНИ ДОЛЖНЫ БЫТЬ ОПРЕДЕЛЁННЫМ ОБРАЗОМ УСТАНОВЛЕНЫ В МАССИВЕ ПОТОМКОВ
    //     for (unsigned int c = 0; c < 8; c++)
    //     {
    //         child = parent->child[c];
    //         segmentPosition = Vector3<unsigned int>(j * , k, l);
            
    //         // Обработка сегмента сетки вокселей и объединение его в один воксель
    //         for (unsigned int x = 0; x < segmentSize.x; x++)
    //             for (unsigned int y = 0; y < segmentSize.y; y++)
    //                 for (unsigned int z = 0; z < segmentSize.z; z++)
    //                 {
    //                     axisVoxel = voxel[segmentPosition.x + x][segmentPosition.y + y][segmentPosition.z + z];

    //                     if (axisVoxel)
    //                     {
    //                         if (parent->voxel)
    //                         {
    //                             dynamic_cast<ComplexMaterial *>(parent->voxel->material)->addMaterial(axisVoxel->material); // Добавляем материал
    //                             parent->voxel->normal += axisVoxel->normal;
    //                         }
    //                         else
    //                             parent->voxel = new Voxel(axisVoxel->normal, new ComplexMaterial(axisVoxel->material));
    //                     }
    //                 }
    //         parent->voxel->normal.normalize();
    //     }
    //     segmentSize = segmentSize / 2;
    // }
}

// Рекурсивное создание элементов-вокселей окто-дерева
// Возвращает указатель на элемент или NULL в случае пустого элемента

// Parent — обрабатываемый элемент дерева, в котором создаётся воксель
// p - позиция обрабатываемого элемента в координатах сетки
// dr - размер вокселя в разрешении сетки на текущей глубине
// voxel - матрица вокселей

Octree * VoxelGeometry::createVoxel(Octree * parent, Vector3<unsigned int> p, unsigned int dr, Voxel *** * voxel)
{
    if (not parent->child) // Если пришли в конец дерева
    {
        if (voxel[p.x][p.y][p.z]) // Если в этом секторе сетки (последнем элементе дерева) есть воксель
            parent->voxel = voxel[p.x][p.y][p.z]; // Записываем его в элемент
        else
        {
            delete parent; // Иначе удаляем текущий элемент
            return NULL;
        }
        return parent;
    }

    Vector3<unsigned int> t; // Переменная для временного хранения смещения относительно родителя
    dr /= 2; // изменение размера вокселя в координатах для обработки потомков

    for (unsigned int c = 0; c < 8; c++) // Последовательный перебор всех потомков элемента
    {
        // Обработка смещения координат для текущего потомка
        t = Vector3<unsigned int>(0, 0, 0);
    
        if (c / 4 == 1) 
            t.x += dr;
        if (c / 2 % 2 == 1)
            t.y += dr;
        if (c % 2 == 1)
            t.z += dr;

        // Рекурсивно вызываем функцию для создания вокселей-потомков
        parent->child[c] = createVoxel(parent->child[c], p + t, dr, voxel); 

        if (parent->child[c]) // Если потомок не пустой
        {
            if (not parent->voxel) // Если воксель в текущем элементе ещё не создан
                parent->voxel = new Voxel(); // Создаём воксель
            parent->voxel->addMaterial(parent->child[c]->voxel->material); // Добавляем его материал к текущему вокселю
            parent->voxel->normal += parent->child[c]->voxel->normal; // Добавляем его нормаль к текущему вокселю
        }
    }
    if (not parent->voxel) // Если все потомки пустые, и вокселя в текущем элементе нет, считаем этот элемент конечным
    {
        delete parent;
        return NULL;
    }
    parent->voxel->normal.normalize(); // Нормализация нормали в случае не пустого вокселя
    return parent;
}

VoxelGeometry::~VoxelGeometry()
{
    // for (unsigned int x = 0; x < resolution.x; x++)
    //     for (unsigned int y = 0; y < resolution.y; y++)
    //         for (unsigned int z = 0; z < resolution.z; z++)
    //             delete voxel[x][y][z];
}

// Voxel * VoxelGeometry::addVoxel(const Vector3<unsigned int> p)
// {
//     voxel[p.x][p.y][p.z] = new Voxel(); // Создаём пустой воксель
//     return voxel[p.x][p.y][p.z];
// }

// Voxel * VoxelGeometry::addVoxel(const Vector3<unsigned int> p, const Vector3<double> n, const Material * m)
// {
//     voxel[p.x][p.y][p.z] = new Voxel(n, m->clone()); // Создаём новый воксель
//     return voxel[p.x][p.y][p.z];
// }

// Voxel * VoxelGeometry::addVoxel(const Vector3<double> p, const Vector3<double> n, const Material * m)
// {
//     Vector3<unsigned int> vp = locate(p);
//     voxel[vp.x][vp.y][vp.z] = new Voxel(n, m->clone()); // Создаём новый воксель
//     return voxel[vp.x][vp.y][vp.z];
// }

// Voxel * VoxelGeometry::getVoxel(const Vector3<unsigned int> & p) const
// {
//     return voxel[p.x][p.y][p.z];
// }

Vector3<unsigned int> VoxelGeometry::locate(const Vector3<double> & p) const
{
    return Vector3<unsigned int>(p.x / scale, p.y / scale, p.z / scale);
}

Vector3<double> VoxelGeometry::getPosition(const Vector3<unsigned int> & p) const
{
    return Vector3<double>(p.x * scale, p.y * scale, p.z * scale);
}

unsigned int VoxelGeometry::getId(unsigned int x, unsigned int y, unsigned int z) const
{
    return x + y * resolution.x + z * resolution.x * resolution.y;
}

// Возвращает количество непустых вокселей
unsigned int VoxelGeometry::count(const Octree * parent) const
{
    if (parent->child)
    {
        unsigned int n = 0;

        for (unsigned int c = 0; c < 8; c++)
            if (parent->child[c])
                n += count(parent->child[c]);

        return n;
    }
    else
        if (parent->voxel)
            return 1;

    return 0;
}

// Рекурсивный вывод вокселей в файл PLY
void VoxelGeometry::printPLY(ofstream & file, const Octree * parent, Vector3<unsigned int> p, unsigned int dr) const
{
    if (parent->child)
    {
        Vector3<unsigned int> t;
        dr /= 2;

        for (unsigned int c = 0; c < 8; c++)
        {
            t = Vector3<unsigned int>(0, 0, 0);
        
            if (c / 4 == 1) 
                t.x += dr;
            if (c / 2 % 2 == 1)
                t.y += dr;
            if (c % 2 == 1)
                t.z += dr;

            if (parent->child[c])
                printPLY(file, parent->child[c], p + t, dr);
        }
    }
    else
        if (parent->voxel)
        {
            RGB color = parent->voxel->material->getDiffuse() * 255;
            Vector3<double> normal = parent->voxel->normal;

            file << (float)(position.x + (scale * p.x)) << ' ' << (float)(position.y + (scale * p.y)) << ' ' << (float)(position.z + (scale * p.z)) << ' ';
            file << (float)normal.x << ' ' << (float)normal.y << ' ' << (float)normal.z << ' ';
            file << (int)color.r << ' ' << (int)color.g << ' ' << (int)color.b << '\n';
        }
}

void VoxelGeometry::exportPLY(const string filename) const
{
    ofstream file(filename);

    if (file.is_open()) 
    {
        unsigned int voxels = count(tree);

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

        printPLY(file, tree, Vector3<unsigned int>(0, 0, 0), resolution.x);
    }
    else
        throw "Incorrect file path!";
    file.close();
}


bool VoxelGeometry::inside(const Vector3<double> & p) const
{
    if ( p.x < position.x or p.x >= end.x or p.y < position.y or p.y >= end.y or p.z < position.z or p.z >= end.z )
        return false;

    return true;
}


double VoxelGeometry::intersect(const Ray & ray) const
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


Point * VoxelGeometry::trace(Ray & ray) const
{
    double length = 0; // Длинна луча до обработнной точки

    if (not inside(ray.position)) // Если луч вне сетки, ищем точку пересечения луча с областью сетки
    {
        length = intersect(ray);

        if (length < 0) // Точка пересечения не нашлась
            return NULL;
    }

    // Положение луча относительно сетки
    Vector3<double> rayPosition = ray.position - position; 

    // Текущая позиция трассировщика относительно сетки
    Vector3<double> tracerPosition = rayPosition + ray.direction * length;

    // Стэк для хранения родителей при обходе дерева
    stack<Octree *> parents;

    // Текущий обрабатываемый родитель
    Octree * parent = tree;

    // Положение родителя относительно сетки
    Vector3<double> parentPosition = Vector3<double>(0, 0, 0);

    // Текущий размер потомков в координатах
    double voxelSize = size.x / 2;

    // Направление в котором будем перемещаться по вокселям (1; -1; 0)
    Vector3<int> direction = Vector3<int>
    (
        sign(ray.direction.x),
        sign(ray.direction.y),
        sign(ray.direction.z)
    );

    // Неотрицательность направления перемещения (0; 1)
    Vector3<int> directionPositive = Vector3<int>
    (
        direction.x > 0,
        direction.y > 0,
        direction.z > 0
    );

    // Расстояние для преодаления целого вокселя-потомка в направлении direction
    Vector3<double> deltaLength = Vector3<double>
    (
        voxelSize / ray.direction.x * direction.x,
        voxelSize / ray.direction.y * direction.y,
        voxelSize / ray.direction.z * direction.z
    );

    // Граница для поиска пересечения луча относительно сетки
    Vector3<double> bound;
    
    // Расстояние до пересечения с плоскостями границы параллельными осям
    Vector3<double> t;

    // Положение потомка в воксельных координатах
    Vector3<int> childVoxelPosition;

    // Координата текущего потомка относительно родителя (ОТНОСИТЕЛЬНО СЕТКИ???) ------------
    Vector3<double> childPosition;

    // Обрабатываемая ось координат: 0 - Х, 1 - Y, 2 - Z
    int axis = 0;
    
    while (true) // Цикл прохода по уровням дерева
    {
        if (parent->child) // Есть ли у родителя потомки?
        {
            // Определение положения потомка в воксельных координатах
            childVoxelPosition = getVoxelPosition(tracerPosition - parentPosition, voxelSize);

            if (childVoxelPosition > 1 or childVoxelPosition < 0)
            {
                // Если произвести подъём нельзя, пересечения не нашлось
                if (parent == tree) return NULL; 

                // Подъём по дереву, переход к родителю
                parent = parents.top();
                parents.pop();
                voxelSize = voxelSize * 2;
                deltaLength = deltaLength * 2;
                parentPosition = getPosition(parentPosition, voxelSize * 2);

                continue;
            }

            // Определение позиции текущего потомка относительно родителя
            childPosition = getPosition(childVoxelPosition, voxelSize);

            // Определение номера потомка в массиве потомков
            int i = parent->getChildIndex(childVoxelPosition);

            // Отдельная обработка первого потомка для ускорения трассировки
            if (parent->child[i])
            {
                // Углубление в дерево, переход к новому родителю — потомку
                parents.push(parent);
                parent = parent->child[i];
                parentPosition = parentPosition + childPosition;
                voxelSize = voxelSize / 2;
                deltaLength = deltaLength / 2;
                continue;
            }

            bound = Vector3<double>
            (
                parentPosition.x + childPosition.x + voxelSize * directionPositive.x + BOUNDARY * direction.x,
                parentPosition.y + childPosition.y + voxelSize * directionPositive.y + BOUNDARY * direction.y,
                parentPosition.z + childPosition.z + voxelSize * directionPositive.z + BOUNDARY * direction.z
            );

            // Определение расстояния до ближайших вокселей
            t = getT(bound, tracerPosition, ray.direction);

            while (true) // Цикл прохода по потомкам, пересекаемым лучом
            {
                // Текущая ось движения
                axis = 0;

                // Определение ближайшего пересечения с плоскостью
                if (t.y < t[axis])
                    axis = 1;
                if (t.z < t[axis])
                    axis = 2;

                // Переход к следующему вокселю по оси координат
                childVoxelPosition[axis] += direction[axis];

                // Выход за пределы родителя, поднимаемся, пока не окажемся внутри нового родителя
                if (childVoxelPosition[axis] > 1 or childVoxelPosition[axis] < 0)
                {
                    // Изменение текущей позиции трассировщика и длинны луча
                    length += t[axis];
                    tracerPosition = tracerPosition + ray.direction * t[axis];

                    break;
                }

                // Определение позиции текущего потомка относительно родителя
                childPosition[axis] = voxelSize * childVoxelPosition[axis];

                // Определение индекса потомка;
                i = parent->getChildIndex(childVoxelPosition);

                if (parent->child[i])
                {
                    // Изменение текущей позиции трассировщика и длинны луча
                    length += t[axis];
                    tracerPosition = tracerPosition + ray.direction * t[axis];

                    // Углубление в дерево, переход к потомку
                    parents.push(parent);
                    parent = parent->child[i];
                    parentPosition = parentPosition + childPosition;
                    voxelSize = voxelSize / 2;
                    deltaLength = deltaLength / 2;
                    break;
                }

                t[axis] += deltaLength[axis];
                bound[axis] += voxelSize * direction[axis];
            }
            
            continue;
        }
        // Если точка ближе предыдущего пересечения
        if(length < ray.length) 
        {
            // Сохраняем в луч найденное расстояние
            ray.length = length; 

            // Смещаем границу в обратную сторону
            bound[axis] -= 2 * BOUNDARY * direction[axis]; 

            // Находим ближайшую точку перед вокселем
            double t = (position[axis] + bound[axis] - ray.position[axis]) / ray.direction[axis];

            return new Point(ray.position + ray.direction * t, parent->voxel->normal, parent->voxel->material);
            // return new Point(ray.position + ray.direction * ray.length, parent->voxel->normal, parent->voxel->material);
        }
        return NULL;
    }
    return NULL;
}

// Octree * descent(stack<Octree *> & parents, Octree * parent, Octree * child, Vector3<double> & parentPosition) const
// {
//     parents.push(parent);
//     parent = parent->child[i];
//     parentPosition = parentPosition + childPosition;
// }

// Определение координат вокселя
// voxelPosition - воксельные координаты вокселя
// size - размер вокселя
Vector3<double> VoxelGeometry::getPosition(const Vector3<int> & voxelPosition, const double & size) const
{
    return Vector3<double>
    (
        voxelPosition.x * size,
        voxelPosition.y * size,
        voxelPosition.z * size
    );
}

// Определение позиции вокселя по точке внутри него
// position - точка внутри вокселя
// size - размер вокселя
Vector3<double> VoxelGeometry::getPosition(const Vector3<double> & position, const double & size) const
{
    return Vector3<double>
    (
        static_cast<unsigned int>(position.x / size) * size,
        static_cast<unsigned int>(position.y / size) * size,
        static_cast<unsigned int>(position.z / size) * size
    );
}

// Определение воксельных координат точки в пространстве
// position - точка в пространстве
// size - размер вокселя
Vector3<int> VoxelGeometry::getVoxelPosition(const Vector3<double> & position, const double & size) const
{
    return Vector3<int>
    (
        position.x / size - (position.x < 0),
        position.y / size - (position.y < 0),
        position.z / size - (position.z < 0)
    );
}

// RAY DIRECTION НЕ может быть равен нулю!!!
Vector3<double> VoxelGeometry::getT(const Vector3<double> & bound, const Vector3<double> & position, const Vector3<double> & direction) const
{
    return Vector3<double>
    (
        (bound.x - position.x) / direction.x, 
        (bound.y - position.y) / direction.y, 
        (bound.z - position.z) / direction.z
    );
}

// Рекурсивная трассировка луча в окто-дереве вокселей
// ray — трассируемый луч, должен изменить свою длинну в результате трассировки
// parentPosition - позиция текущего вокселя относительно начала сетки
// voxelSize - размер вокселя на текущем уровне дерева в глобальных координатах
// Point * VoxelGeometry::trace(Ray & ray, const Octree * parent, Vector3<double> parentPosition, double voxelSize, double length) const
// {
//     if (parent->child)
//     {
//         voxelSize /= 2; // изменение размера вокселя в координатах для обработки потомков

//         // Текущая позиция трассировщика относительно обрабатываемого вокселя
//         Vector3<double> tracerPosition = ray.position + ray.direction * length - parentPosition - position;

//         // Определение позиции текущего потомка в воксельных координатах
//         Vector3<unsigned int> childVoxelPosition = Vector3<unsigned int>
//         (
//             tracerPosition.x / voxelSize,
//             tracerPosition.y / voxelSize,
//             tracerPosition.z / voxelSize
//         );

//         // Определение позиции текущего потомка относительно родителя/сетки
//         Vector3<double> childPosition = Vector3<double>
//         (
//             childVoxelPosition.x * voxelSize,
//             childVoxelPosition.y * voxelSize,
//             childVoxelPosition.z * voxelSize
//         );

//         unsigned int i = parent->getChildIndex(childVoxelPosition);

//         // Отдельная обработка первого потомка для ускорения трассировки
//         if (parent->child[i])
//         {
//             Point * t = trace(ray, parent->child[i], parentPosition + childPosition, voxelSize, length);
//             t->material->getDiffuse();
//             if (t) return t; // В случае нахождения точки пересечения возвращаем её
//         }
//         // Направление в котором будем перемещаться по вокселям (1; -1; 0)
//         Vector3<int> direction = Vector3<int>
//         (
//             sign(ray.direction.x),
//             sign(ray.direction.y),
//             sign(ray.direction.z)
//         );

//         // Граница для поиска пересечения луча
//         Vector3<double> bound(0, 0, 0);

//         if (direction.x > 0)
//             bound.x += voxelSize;
//         if (direction.y > 0)
//             bound.y += voxelSize;
//         if (direction.z > 0)
//             bound.z += voxelSize;

//         // Расстояние до пересечения с вокселями соответствующих направлений
//         Vector3<double> t = Vector3<double>
//         (
//             (bound.x - tracerPosition.x) / ray.direction.x * direction.x, 
//             (bound.y - tracerPosition.y) / ray.direction.y * direction.y, 
//             (bound.z - tracerPosition.z) / ray.direction.z * direction.z
//         );

//         // Расстояние для преодоления целого вокселя в направлении
//         Vector3<double> deltaLength = Vector3<double>
//         (
//             voxelSize / ray.direction.x * direction.x,
//             voxelSize / ray.direction.y * direction.y,
//             voxelSize / ray.direction.z * direction.z
//         );

//         // Текущая ось координат: 0 - Х, 1 - Y, 2 - Z
//         int axis;

//         while (true) // Цикл прохода по сетке вокселей
//         {
//             axis = 0;

//             if (t.y < t[axis])
//                 axis = 1;
//             if (t.z < t[axis])
//                 axis = 2;

//             // Переход к следующему вокселю по оси координат
//             childVoxelPosition[axis] += direction[axis];

//             // Выход за пределы родителя
//             if (childVoxelPosition[axis] > 1 or childVoxelPosition[axis] < 0)
//                 return NULL;

//             // Определение позиции текущего потомка относительно родителя/сетки
//             Vector3<double> childPosition = Vector3<double>
//             (
//                 parentPosition.x + childVoxelPosition.x * voxelSize,
//                 parentPosition.y + childVoxelPosition.y * voxelSize,
//                 parentPosition.z + childVoxelPosition.z * voxelSize
//             );

//             i = parent->getChildIndex(childVoxelPosition);

//             if (parent->child[i])
//             {
//                 length += t[axis];
//                 Point * t = trace(ray, parent->child[i], parentPosition + childPosition, voxelSize, length);
//                 t->material->getDiffuse();
//                 if (t) return t; // В случае нахождения точки пересечения возвращаем её
//             }

//             t[axis] += deltaLength[axis];
//         }
//     }

//     if(length < ray.length) // И она ближе предыдущей
//     {
//         ray.length = length; // Сохраняем в луч найденное расстояние
//         Point * t = new Point(ray.position + ray.direction * ray.length, parent->voxel->normal, parent->voxel->material);
//         t->material->getDiffuse();
//         return t;
//     }
// }
