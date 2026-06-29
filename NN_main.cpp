#include <algorithm>
#include "init.hpp"
#include "run.hpp"
#include "uw.hpp"
#include "Softmax.hpp"

constexpr double init_lr = 0.01;
constexpr double PI = 3.1415926535;

static inline vector<double> sp(const vector<double>& x) {
	vector<double> ret(x.size());
	for (size_t i = 0; i < x.size(); i++) { ret[i] = sigmoid(x[i]); }
	return ret;
}

runc NN(const vector<vector<double>>& input, const vector<double>& tv, initc& c,
	size_t _step_, int step, const vector<int>& neur, u32string m,
	int N, int M) {
	if (m == U"go") {
		vector<double> mrs;
		mrs.reserve(_step_);
		double lr = init_lr;
		int T = (int)std::max<size_t>(85, _step_ / 2);
		double se = 0.0;
		cout << "\n";

		for (size_t i = 0; i < _step_; i++) {
			runc rc = run(c, input, neur, step, N, M, 0.05);
			if (!rc.error.empty()) { return rc; }
			vector<double> opsm = sp(rc.output);

			uw(c, input, rc.ha, rc.hp, opsm, neur, step, tv, M, N, lr);

			double mr = mse(opsm, tv);
			mrs.push_back(mr);
			se += mr;

			// 单调衰减学习率，避免 cosine 周期导致后期震荡
			lr = init_lr / (1.0 + 0.001 * (double)i);
			lr = std::max(lr, init_lr * 0.01);
			//lr *= 1.2;

			// 每隔 100 步打印一次日志，避免输出过多拖慢训练
			if (_step_ >= 10 && (i % 100 == 0 || i == _step_ - 1)) {
				double yes = nyes(opsm, tv);
				cout << "[tmp-log]mse = " << mr << "\tyes = " << yes
				     << "\tDA = " << std::tanh(yes / std::min(opsm.size(), tv.size()))
				     << "\tstep = " << i << endl;
			}

			// 早停：连续多次提升明显则结束本轮训练
			if (i > 30 && mr > 10.0) { cout << "[log]提前结束" << endl; break; }
		}
		cout << "[log]本段平均mse结果= " << se / mrs.size() << endl;
		#if 0
		{
			for (size_t i = 0; i < yes.size(); i++) { cout << yes[i] << endl; }
			cout << "最终lr：" << lr << endl;
			cout << "平均正确度：" << se / yes.size() << endl;
		}
		#endif
	}
	else if (m == U"end") { cout << "结束......"; return runc(); }
	else { runc rc; rc.error.push_back(U"未知模式"); return rc; }
	return runc();
}
