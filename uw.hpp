#ifndef uw_hpp
#define uw_hpp

#include <algorithm>
#include "header.hpp"

inline double nyes(const vector<double>& p, const vector<double>& tv) {
	double sum = 0;
	size_t n = std::min(p.size(), tv.size());
	for (size_t i = 0; i < n; i++) {
		// 每个输出维度：预测完全正确为 +1，完全错误为 -1
		double d = 1.0 - 2.0 * std::abs(tv[i] - p[i]);
		sum += d;
	}
	return sum;
}

inline double mse(const vector<double>& p, const vector<double>& tv) {
	double sum = 0;
	size_t n = std::min(p.size(), tv.size());
	for (size_t i = 0; i < n; i++) {
		double d = tv[i] - p[i];
		sum += d * d;
	}
	return sum;
}

inline void clip_weights(initc& c, double min_v, double max_v) {
	for (auto& row : c.iw) for (auto& w : row) w = std::max(min_v, std::min(max_v, w));
	for (auto& row : c.hw) for (auto& w : row) w = std::max(min_v, std::min(max_v, w));
	for (auto& row : c.ow) for (auto& w : row) w = std::max(min_v, std::min(max_v, w));
	for (auto& b : c.b) b = std::max(min_v, std::min(max_v, b));
	for (auto& ob : c.ob) ob = std::max(min_v, std::min(max_v, ob));
}

inline void decay_weights(initc& c, double factor) {
	for (auto& row : c.iw) for (auto& w : row) w *= factor;
	for (auto& row : c.hw) for (auto& w : row) w *= factor;
	for (auto& row : c.ow) for (auto& w : row) w *= factor;
	for (auto& b : c.b) b *= factor;
	for (auto& ob : c.ob) ob *= factor;
}

inline void uw(initc& c,
	const vector<vector<double>>& input,
	const vector<vector<double>>& ha,
	const vector<vector<double>>& hp,
	const vector<double>& output,
	const vector<int>& neur, int step, const vector<double>& tv, int M, int N,
	double lr) {
	const size_t hidden = neur[1];
	const size_t feed = hidden - M;
	const size_t output_neurons = neur[2];
	const size_t input_neurons = neur[0];

	// 全局多巴胺强度，映射到 [0,1]，预测越好强度越高
	const double dim = static_cast<double>(std::min(output.size(), tv.size()));
	double yes = nyes(output, tv);
	double DA = (std::tanh(yes / dim) + 1.0) * 0.5;

	// 输出层局部误差方向
	vector<double> r(output_neurons, 0.0);
	for (size_t i = 0; i < output_neurons; i++) {
		double target = (i < tv.size()) ? tv[i] : 0.0;
		r[i] = target - output[i];
	}

	const size_t last_idx = static_cast<size_t>(step) - 1;
	const vector<double>& last_h = ha[last_idx];

	// 输出层权重与偏置更新
	for (size_t i = 0; i < output_neurons; i++) {
		for (size_t j = 0; j < feed; j++) {
			c.ow[i][j] += lr * DA * r[i] * last_h[j];
		}
		c.ob[i] += lr * DA * r[i];
	}

	// 将输出误差线性投影回隐藏层，作为隐藏层更新方向
	vector<double> hidden_reward(hidden, 0.0);
	for (size_t j = 0; j < feed; j++) {
		double sum = 0.0;
		for (size_t i = 0; i < output_neurons; i++) {
			sum += r[i] * c.ow[i][j];
		}
		hidden_reward[j] = sum;
	}
	// 递归部分无直接输出连接，投影保持为零

	// 沿整个时间步累积更新输入层与隐藏层
	for (int t = 0; t < step; t++) {
		const size_t cur = static_cast<size_t>(t);
		const vector<double>& h = ha[cur];
		const vector<double>& inp = input[cur];

		// 输入层权重与偏置更新
		for (size_t i = 0; i < feed; i++) {
			double post_dir = hidden_reward[i] * (1.0 - h[i] * h[i]); // tanh 导数
			for (size_t j = 0; j < input_neurons && j < inp.size(); j++) {
				c.iw[i][j] += lr * DA * post_dir * inp[j];
			}
			c.b[i] += lr * DA * post_dir;
		}

		// 隐藏层权重与偏置更新
		if (t > 0) {
			const vector<double>& prev_h = ha[cur - 1];
			for (size_t i = 0; i < hidden; i++) {
				double post_dir = hidden_reward[i] * (1.0 - h[i] * h[i]);
				for (size_t j = 0; j < hidden; j++) {
					c.hw[i][j] += lr * DA * post_dir * prev_h[j];
				}
				// 前馈偏置已在输入层更新，此处仅更新递归部分偏置
				if (i >= feed) {
					c.b[i] += lr * DA * post_dir;
				}
			}
		}
		else {
			// t=0 无前一时刻隐藏状态，仅更新递归部分偏置
			for (size_t i = feed; i < hidden; i++) {
				double post_dir = hidden_reward[i] * (1.0 - h[i] * h[i]);
				c.b[i] += lr * DA * post_dir;
			}
		}
	}

	clip_weights(c, -2.0, 2.0);
}

#endif
