#include "Point.hpp"
#include "Scene.hpp"
#include "Constants.hpp"
#include "triangleArea.hpp"

RGB Point::calcIlluminance(const Scene & scene) const
{
    RGB e, i;
    Light * light;
    Vector3 lightVector;

    for (unsigned long n = 0; n < scene.light.size(); ++n) // по всем источникам накапливаем освещенность
    {
        light = scene.light[n]; // Текущий источник

        lightVector = light->getPosition() - position; // Вектор от точки к источнику

        double cos = cosVector(lightVector, normal); // Косинус между вектороми нормали и света
        double r = lightVector.length(); // Расстояние от источника до точки

        Ray shadowRay = Ray(lightVector, position + (lightVector * PRECISION));

        if (scene.traceGeometry(shadowRay)) // Проверка на осветимость точки текущим источником
            if (shadowRay.length < r + PRECISION) // Если найдена точка пересечения луча с поверхностью между источником и точкой
                continue; // Переходим к обработке следующего источника

        if (cos > 0 - PRECISION) // Если свет падает не с обратной стороны поверхности 
        {
            i = light->getIntencity(-lightVector); // Получаем силу света в направлении от источника к точке
            e = e + i * (cos / (r * r)); // Складываем освещенности от источников
        }
    }
    return e;
}

RGB Point::calcIlluminance(const Light & light, const Scene & scene) const
{
    RGB e, i;

    Vector3 lightVector = light.position - position; // Вектор от точки к источнику

    double cos = cosVector(lightVector, normal); // Косинус между векторами нормали и света
    double r = lightVector.length(); // Расстояние от источника до точки

    Ray shadowRay = Ray(lightVector, light.position); // Теневой луч

    if (scene.trace(shadowRay)) // Проверка на осветимость точки текущим источником
        if (shadowRay.length < r + PRECISION) // Если найдена точка пересечения луча с поверхностью между источником и точкой
            return e; // Возвращаем 0

    if (cos > 0 - PRECISION) // Если свет падает не с обратной стороны поверхности 
    {
        i = light.getIntencity(-lightVector); // Получаем силу света в направлении от источника к точке
        e = e + i * (cos / (r * r)); // Складываем освещенности от источников
    }
    return e;
}