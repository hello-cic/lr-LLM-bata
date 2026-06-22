#ifndef run_hpp
#define run_hpp

#include "header.hpp"

inline double sigmoid(double x) {return 1.0 / (1.0 + exp(0 - x));}

inline double silu(double x) {return x * sigmoid(x);}

inline vector<double> inita(int neurs, 
const vector<double> ip, 
const vector<double> b, 
const vector<vector<double>> w) {
	vector<double> inita;
	for (int i = 0; i < neurs; i++) {
		double sum = 0.0;
		for (size_t n = 0; n < ip.size(); n++) { sum += ip[n] * w[i][n];}
		sum = abs(sum - b[i]);
		inita.push_back(sum);
	}
	return inita;
}
inline runc run(vector<vector<double>>& iw,
vector<vector<double>>& hw,
vector<double>& b, 
vector<vector<double>> ow,
const vector<vector<double>> input,
const vector<int> neur, int step, int N, int M, double rr) {
	runc rc;
	if (N + M != neur[1]) {
		if (N + M < neur[1]) { rc.error.push_back(U"Please increase the number of hidden layers."); }
		if (N + M > neur[1]) { rc.error.push_back(U"Please decrease the number of hidden layers."); }
		return rc;
	}
	vector<double> ip(neur[1], 0.0);
	vector<double> op(ip.size());
	vector<double> r(N, 0.0);
	for (size_t step_ = 0; step_ < step; step_++) {
		vector<double> _initha_ = inita(neur[1] - M, input[step_], b, iw);
		op = _initha_;
		op.insert(op.end(), r.begin(), r.end());
		for (size_t i = 0;i < neur[1];i++) {
			double sum = 0;
			for (size_t n = 0; n < neur[1]; n++) { sum += op[n] * hw[i][n]; }
			if (i < N) {
				if (sum < 0.2) { b[i] = (b[i] * 0.7) * (sum * 0.3); }
				else { b[i] *= 0.7; }
				b[i] = std::min(b[i], 5.0);
			}
			for (size_t n = 0; n < neur[1]; n++) { hw[i][n] -= sum * 0.000000001 / step; }
			sum /= b[i];
			sum = tanh(sum);
			op[i] = sum;
		}
		vector<double> tmpr(op.end() - r.size(), op.end());
		r = jp(cpp(rr, r), cpp((1.0 - rr), tmpr));
		rc.ha.push_back(op);
		op.resize(op.size() - r.size());
		ip = op;
	}
	vector<double> ob;
	for (size_t i = 0;i < neur[2];i++) { ob.push_back(0.0); }
	rc.output = inita(neur[2], op, ob, ow);
	return rc;
}

#endif