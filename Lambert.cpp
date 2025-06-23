#include "Lambert.hpp"
#include "Emissive.hpp"
#include "ComplexMaterial.hpp"
#include <iostream>

RGB Lambert::luminance(const Point & p, Ray & ray, const Scene & scene) const // Луч передаввать по ссылке, и рекурсивно не создавать
{
    RGB e = p.calcIlluminance(scene); // Рассчет освещенности в точке от всех источников

    RGB l = e * color / M_PI * ray.color; // Подсчет яркости в точке

    double m = random<double>(0, 2 * M_PI); // Случайный угол полусферы
    double h = random<double>(-1, 1); // Случайная высота на полусфере

    double s = sqrt(1 - h * h);
    Vector3 d = Vector3(s * cos(m), s * sin(m), h); // Случайная точка на полусфере

    // Vector3 i = p.normal.normalized(); // Один из векторов нового базиса это нормаль к поверхности
    // Vector3 j = Vector3( i.x, i.y, -(i.x * i.x + i.y * i.y) / i.z ).normalized(); // Перпендикулярный нормали вектор
    // Vector3 k = vectorMultiply(i, j).normalized(); // Векторное произведение для нахождение перпендикулярного двум вектора
    
    ray.position = p.position;
    ray.direction = d + p.normal; // Равномерное распределение по интенсивности
    ray.color = ray.color * color;
    ray.length = RAY_CLIP;

    return l; // Возвращение яркости
}

RGB Lambert::getDiffuse() const
{
    return color;
}

Material * Lambert::clone() const
{
    return new Lambert( this->color, this->name);
}

Material * Lambert::addMaterial(const Material * m)
{
    if (name == m->name and m->name != "")
        return this;
    
    Material * newMaterial = new ComplexMaterial(this);
    newMaterial->addMaterial(m);
    return newMaterial;
}