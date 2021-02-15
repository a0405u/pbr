#include "Headers.hpp"

using namespace std;

vector<string> getWords(const string & s, char d)
{
    istringstream ss(s);
    string word;
    vector<string> v;

    while (getline(ss, word, d)) 
    {
        v.push_back(word);
    }
    
    return v;
}

// double random()
// {
//     return (double)rand() / RAND_MAX;
// }

double random(double min, double max)
{
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}
