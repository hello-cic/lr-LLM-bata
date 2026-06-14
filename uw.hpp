#ifndef uw_hpp
#define uw_hpp

#include "header.hpp"

double tmp(const vector<double> p, const vector<double> tv) {
	double sum = 0;
	for (size_t i = 0; i < p.size(); i++) {
		sum += (tv[i] - (softads(tv[i] - p[i]))) * (tv[i] - (softads(tv[i] - p[i])));
	}
	return sum;
}

void uw(vector<vector<double>>& iw, vector<vector<double>>& hw, vector<double>& b, vector<vector<double>>& ow,
const vector<vector<double>> input, const vector<vector<double>> ha, const vector<double> output, 
const vector<int> neur, int step, vector<double> tv, int M, int N,
double lr) {
	double yes = tmp(output, tv);
	for (int step_ = 0; step_ < step; step_++) {
		for (int i = 0; i < neur[1] - M; i++) {
			for (int n = 0; n < neur[0]; n++) {iw[i][n] += ((ha[step_][i] * input[step_][n]) * yes) * lr;}
		}
	}

	for (int step_ = 0; step_ < step; step_++) {
		for (int i = 0; i < neur[1]; i++) {
			for (int n = 0; n < neur[1]; n++) { hw[i][n] += ((ha[step_][i] * ha[step_][n]) * yes) * lr; }
		}
	}

	for (int step_ = 0; step_ < step; step_++) {
		for (int i = 0; i < neur[1]; i++) {
			b[i] += (ha[step_][i] * yes) * lr;
		}
	}

	for (int i = 0; i < neur[1]; i++) {
		for (int n = 0; n < neur[1]; n++) { hw[i][n] += ((ha[step - 1][i] * output[n]) * yes) * lr; }
	}
}

#endif