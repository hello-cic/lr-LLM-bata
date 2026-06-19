#ifndef Softmax_hpp
#define Softmax_hpp

#include <vector>
using std::vector;
#include <cmath>
using std::exp;

inline vector<double> softmax(vector<double> ip) {
	vector<double> op(ip.size());
	double sum = 0;
	for (size_t i = 0; i < ip.size(); i++) { sum += exp(ip[i]); }
	for (size_t i = 0; i < ip.size(); i++) {
		op[i] = exp(ip[i]) / sum;
	}
	return op;
}

#endif