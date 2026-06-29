#ifndef run_hpp
#define run_hpp

#include "header.hpp"

inline double sigmoid(double x) {
	if (x >= 0) {
		return 1.0 / (1.0 + exp(-x));
	}
	else {
		double exp_x = exp(x);
		return exp_x / (1.0 + exp_x);
	}
}

inline double silu(double x) { return x * sigmoid(x); }

inline runc run(initc& c,
	const vector<vector<double>>& input,
	const vector<int>& neur, int step, int N, int M, double rr) {
	runc rc;
	if (N + M != neur[1]) {
		if (N + M < neur[1]) { rc.error.push_back(U"Please increase the number of hidden layers."); }
		if (N + M > neur[1]) { rc.error.push_back(U"Please decrease the number of hidden layers."); }
		return rc;
	}

	const size_t hidden = neur[1];
	const size_t input_neurons = neur[0];
	const size_t output_neurons = neur[2];
	const size_t feed_neurons = hidden - M;

	vector<double> op(hidden);
	vector<double> r(M, 0.0);
	vector<double> tmpr(M);
	vector<double> pre(hidden);

	rc.ha.reserve(step);
	rc.hp.reserve(step);

	for (int step_ = 0; step_ < step; step_++) {
		// 前馈输入层：加权求和加偏置
		for (size_t i = 0; i < feed_neurons; i++) {
			double sum = 0.0;
			for (size_t n = 0; n < input_neurons && n < input[step_].size(); n++) {
				sum += input[step_][n] * c.iw[i][n];
			}
			sum += c.b[i];
			op[i] = sum;
		}
		// 复制上一时刻递归状态
		for (size_t i = feed_neurons; i < hidden; i++) { op[i] = r[i - feed_neurons]; }

		// 隐藏层：加权求和加偏置后经 tanh 激活
		for (size_t i = 0; i < hidden; i++) {
			double sum = 0.0;
			for (size_t n = 0; n < hidden; n++) { sum += op[n] * c.hw[i][n]; }
			sum += c.b[i];
			pre[i] = sum;
			op[i] = tanh(sum);
		}

		for (size_t i = 0; i < M; i++) { tmpr[i] = op[feed_neurons + i]; }
		r = jp(cpp(rr, r), cpp((1.0 - rr), tmpr));
		rc.ha.push_back(op);
		rc.hp.push_back(pre);
	}

	// 输出层：加权求和加输出偏置，sigmoid 在外部应用
	rc.output.resize(output_neurons);
	for (size_t i = 0; i < output_neurons; i++) {
		double sum = 0.0;
		for (size_t n = 0; n < feed_neurons; n++) { sum += op[n] * c.ow[i][n]; }
		sum += c.ob[i];
		rc.output[i] = sum;
	}
	return rc;
}

#endif
