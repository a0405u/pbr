#include "face.hpp"

using namespace std;

// Конструктор Face из строки
Face::Face(const string s, const vector<Vertex *> & vp) 
{
        vector<string> vs = getwords(s); // Получаем строки типа "v/vt/vn" где vt и vn опциональны
        string temp; // Строка для хранения номера v, vt или vn 

        for (vector<string>::iterator it = vs.begin(); it != vs.end(); ++it) //перебираем все вершины
        {
            temp = ""; // попробовать функцию getwords для получения всех, хотя она скорее всего не подойдет

            for (int i = 0; (*it)[i] != '/' && (*it)[i] != *(*it).end(); i++)
                temp += (*it)[i];

            v.push_back(vp[stoi(temp) - 1]);
        }
            
            
}
/*
ostream& operator << (ostream& os, const Face& f)
    {
        for (vector<Vertex *>::iterator it = a.begin(); it != a.end(); ++it)
            os << *it << ' ';
        return os;
    }
*/