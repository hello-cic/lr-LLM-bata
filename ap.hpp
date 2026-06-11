#ifndef ap_hpp
#define ap_hpp

#include <vector>
using std::vector;

vector<double> ap(vector<double> a, 
vector<double> b) {
	vector<double> h;
	for (size_t i = 0; i < a.size(); i++) {h.push_back(a[i] + b[i]);}
	return h;
}

#endif