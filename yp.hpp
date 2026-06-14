#ifndef ap_hpp
#define ap_hpp

#include <vector>
using std::vector;
#include <cmath>
using std::sqrt;

vector<double> jp(const vector<double> a, const vector<double> b) {
	vector<double> h;
	for (size_t i = 0; i < a.size(); i++) { h.push_back(a[i] * b[i]); }
	return h;
}
vector<double> cpp(const double a, 
const vector<double> b) {
	vector<double> h;
	for (size_t i = 0; i < b.size(); i++) {h.push_back(a * b[i]);}
	return h;
}

double softads(double x, double eps = 0.1) {return sqrt(x * x + eps);}

#endif