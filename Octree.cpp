#include "Octree.hpp"

Octree::Octree()
{
    child = NULL;
    parent = NULL;
    voxel = NULL;
}

Octree::Octree(unsigned int d)
{
    parent = NULL; // Пока не нужен
    voxel = NULL;

    if (d == 0) // Конец дерева без потомков
        child = NULL;
    else
    {
        child = new Octree * [8]; // Создание массива потомков

        for (unsigned int c = 0; c < 8; c++) // Создание деревьев в потомках
            child[c] = new Octree(d - 1); // Рекурсивно
    }

}

Octree::Octree(Octree ** c)
{
    child = c;
    parent = NULL;
    voxel = NULL;
}

Octree::Octree(Voxel * v)
{
    child = NULL;
    parent = NULL;
    voxel = v;
}

Octree::~Octree()
{
    if (child)
        for (unsigned int c = 0; c < 8; c++)
            if (child[c])
                delete child[c];

    if (voxel)
        delete voxel;
}

unsigned int Octree::getChildIndex(const Vector3<int> & p) const
{
    int i = 0;

    if (p.x == 1)
        i += 4;
    if (p.y == 1)
        i += 2;
    if (p.z == 1)
        i += 1;
    
    return i;
}

Vector3<unsigned int> Octree::getChildPosition(unsigned int i) const
{
    Vector3<unsigned int> p(0, 0, 0);

    if (i / 4 == 1) 
        p.x += 1;
    if (i / 2 % 2 == 1)
        p.y += 1;
    if (i % 2 == 1)
        p.z += 1;

    return p;
}