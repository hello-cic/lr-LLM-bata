#ifndef init_hpp
#define init_hpp

#include "header.hpp"

inline initc init(const vector<int>& neur, int M, double x) {
	initc c;
	srand((unsigned int)time(nullptr));
	// 参数合法性检查，防止 (size_t)负数 下溢成天文数字
	if (M > neur[1]) {
		c.log.push_back(U"错误：M 不能大于隐藏层神经元数 neur[1]");
		return c;
	}
	//输入层权重 (隐藏层神经元数 × 输入层神经元数)
	c.iw.resize(static_cast<size_t>(neur[1] - M));
	for (size_t i = 0; i < c.iw.size(); i++) {
		c.iw[i].resize(neur[0], 1.0);
		for (size_t n = 0; n < c.iw[i].size(); n++) { double x2 = rand() * 1.0 / RAND_MAX * x; c.iw[i][n] += x2; }
	}

	//隐藏层权重
	c.hw.resize(neur[1]);
	for (size_t i = 0; i < c.hw.size(); i++) {
		c.hw[i].resize(neur[1], 1.0);
		for (size_t n = 0; n < c.hw[i].size(); n++) { double x2 = rand() * 1.0 / RAND_MAX * x; c.hw[i][n] += x2; }
	}
	//隐藏层阈值，远离 0 避免除零
	c.b.resize(neur[1], -1.0);

	//输出层权重
	c.ow.resize(neur[2]);
	for (size_t i = 0; i < c.ow.size(); i++) { c.ow[i].resize(neur[1], 1.0); }
	for (int i = 0; i < neur[2]; i++) {
		for (size_t n = 0; n < c.ow[i].size(); n++) { double x2 = rand() * 1.0 / RAND_MAX * x;c.ow[i][n] += x2; }
	}
	//输出层阈值
	c.ob.resize(neur[2], 0.0);
	return c;
}

#endif