#ifndef NN_main_hpp
#define NN_main_hpp

#include "header.hpp"
int NN(vector<vector<double>> input, vector<double> tv, initc& c,
size_t _step_, int step, vector<int> neur = { 5, 20, 5 }, u32string m = U"go",
int N = 10, int M = 10);

#endif