#ifndef HEADERS_H
#define HEADERS_H

#include <string>
#include <vector>
#include <sstream>
#include <random>
 
using namespace std;

vector<string> getWords(const string & s, char d = ' ');
template<typename Numeric, typename Generator = std::mt19937> Numeric random(Numeric from, Numeric to);
template <typename Ttype> int sign(Ttype val);

#endif