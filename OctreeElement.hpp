#ifndef OCTREEEL_H
#define OCTREEEL_H

using namespace std;

class OctreeElement
{
    private:
    OctreeElement * child[8]; // Возможно необходимо использование Void **
    OctreeElement * parent; // Пока кажется ненужным
    Voxel * voxel; // Каждый элемент дерева может хранить воксель, усредняющий все включающие, или содержать NULL

    public:
    
};

#endif