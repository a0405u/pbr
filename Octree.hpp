#ifndef OCTREE_H
#define OCTREE_H

#include "Voxel.hpp"

using namespace std;

class Octree
{
    public:

    Octree ** child; // Возможно необходимо использование Void **, чтобы хранить воксель в случае последнего уровня дерева
    Octree * parent; // Пока кажется ненужным
    Voxel * voxel; // Каждый элемент дерева может хранить воксель, усредняющий все включающие, или содержать NULL

    Octree(); // Создание пустого дерева
    Octree(unsigned int d); // Создание дерева указанной глубины
    Octree(Voxel * v); // Создание дерева с вокселем
    Octree(Octree ** c); // Создание дерева с массивом детей
    ~Octree();

    unsigned int getChildIndex(const Vector3<int> & p) const;
    Vector3<unsigned int> getChildPosition(unsigned int i) const;
};

#endif