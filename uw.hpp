#ifndef uw_hpp
#define uw_hpp

#include "header.hpp"
void uw(vector<vector<double>>& iw, vector<vector<double>>& hw, vector<double>& hb, vector<vector<double>>& ow,
const vector<vector<double>> input, const vector<vector<double>> ha, const vector<double> output, 
const vector<double> neur, int step, double yes, int M, int N) {
	for (int step_ = 0; step_ < step; step_++) {
		for (size_t i = 0; i < neur[1] - M; i++) {
			for (int n = 0; n < neur[0]; n++) {iw[i][n] += (ha[step_][i] * input[step_][n]) * yes;}
		}
	}

	for (int step_ = 0; step_ < step; step_++) {
		for (size_t i = 0; i < neur[1]; i++) {
			for (int n = 0; n < neur[1]; n++) { hw[i][n] += (ha[step_][i] * ha[step_][n]) * yes; }
		}
	}

	for (size_t i = 0; i < neur[1]; i++) {
		for (int n = 0; n < neur[1]; n++) { hw[i][n] += (ha[step - 1][i] * output[n]) * yes; }
	}
}

#endif