#include "Lambert.hpp"
#include <iostream>

RGB Lambert::luminance(Point p, Ray o, const Scene & scene) // Луч передаввать по ссылке, и рекурсивно не создавать
{
    RGB e = p.calcIlluminance(scene); // Рассчет освещенности в точке от всех источников

    RGB l = e * color / M_PI; // Подсчет яркости в точке

    double m = random(0, 2 * M_PI); // Случайный угол полусферы
    double h = random(-1, 1); // Случайная высота на полусфере

    double s = sqrt(1 - h * h);
    Vector3 d = Vector3(s * cos(m), s * sin(m), h); // Случайная точка на полусфере

    // Vector3 i = p.normal.normalized(); // Один из векторов нового базиса это нормаль к поверхности
    // Vector3 j = Vector3( i.x, i.y, -(i.x * i.x + i.y * i.y) / i.z ).normalized(); // Перпендикулярный нормали вектор
    // Vector3 k = vectorMultiply(i, j).normalized(); // Векторное произведение для нахождение перпендикулярного двум вектора
    
    d = d + p.normal; // Равномерное распределение по интенсивности

    Ray r = Ray(d, p.position, o.color * color, o.count - 1); // Создание луча из точки p в направлении d

    if (r.color != 0 or r.count <= 0)  // Если луч не полностью поглотился и у него не закончился счетчик переотражений
    {
        Point * np = scene.trace(r); // Находим точку пересечения луча и геометрии

        if (np) // Если точка существует
            l = l + np->material->luminance(*np, r, scene) ; // Дальше трассируем луч
    }

    return l * o.color; // Возвращение яркости
}

RGB Lambert::getColor()
{
    return color;
}