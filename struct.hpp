#ifndef struct_hpp
#define struct_hpp

#include <vector>
using std::vector;

#include <string>
using std::u32string;

struct initc {
	vector<vector<double>> iw;//输入层权重
	vector<vector<double>> hw;//隐藏层权重
	vector<double> b;//隐藏层阈值
	vector<vector<double>> ow;//输出层权重
	vector<u32string> log;//日志
};

struct runc {
	vector<vector<double>> ia;
	vector<vector<double>> ha;
	vector<double> output;
	vector<u32string> error;
};

#endif