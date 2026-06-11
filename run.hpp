#ifndef run_hpp
#define run_hpp

#include "header.hpp"
vector<double> initha(int neur1, 
const vector<double> ip, 
const vector<double> b, 
const vector<vector<double>> w) {
	vector<double> initha;
	for (int i = 0; i < neur1; i++) {
		double sum = 0.0;
		for (size_t n = 0; n < ip.size(); n++) {sum += ip[n] * w[i][n];}
		sum = max(sum - b[i], 0.0);
		initha.push_back(sum);
	}
	return initha;
}
runc run(vector<vector<double>>& iw,
vector<vector<double>> hw,
vector<double> b, 
vector<vector<double>> ow,
const vector<double> input,
const vector<int> neur, int step) {
	runc rc;
	vector<double> op;
	vector<double> ip;
	for (size_t step_ = 0; step_ < step; step_++) {
		vector<double> _initha_ = initha(neur[1], input, b, iw);
		op = ap(ip, _initha_);
		for (size_t i = 0;i < neur[1];i++) {
			double sum = 0;
			for (size_t n = 0; n < neur[1]; n++) { sum += op[n] * hw[i][n]; }
			op[i] = sum;
		}
		ip = op;
	}
	return rc;
}

#endif