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

// double random(double min, double max)
// {
//     double f = (double)rand() / RAND_MAX;
//     return min + f * (max - min);
// }

template<typename Numeric, typename Generator>
Numeric random(Numeric from, Numeric to)
{
    thread_local static Generator gen(std::random_device{}());

    using dist_type = typename std::conditional
    <
        std::is_integral<Numeric>::value
        , std::uniform_int_distribution<Numeric>
        , std::uniform_real_distribution<Numeric>
    >::type;

    thread_local static dist_type dist;

    return dist(gen, typename dist_type::param_type{from, to});
}

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

template int sign<double>(double val);
template double random<double, std::mt19937>(double from, double to);
