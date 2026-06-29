#include "NN.hpp"
#include "my_class.hpp"
#define NOMINMAX
#include <windows.h>
#include <locale>
#include <algorithm>
#include <fstream>

static inline vector<double> sp(vector<double> x) {
	vector<double> ret(x.size());
	for (size_t i = 0; i < x.size(); i++) { ret[i] = sigmoid(x[i]); }
	return ret;
}

static int findp(vector<char32_t> a, char32_t b) {
	int ret = -1;
	for (int i = 0; i < a.size(); i++) { if (a[i] == b) { ret = (int)i; break; } }
	return ret;
}

static vector<vector<double>> take(const vector<vector<double>>& a, int b) {
	vector<vector<double>> ret;
	if (b <= 0) { return ret; }
	int n = std::min(b, (int)a.size());
	for (int i = 0; i < n; i++) { ret.push_back(a[i]); }
	return ret;
}

static vector<double> vitovd(vector<int> input) {
	vector<double> output(input.size());
	for (size_t i = 0; i < input.size(); i++) {
		output[i] = std::stod(to_string(input[i]) + string(".0")); //变成x.0（这个是double/float）
	}
	return output;
}

int main() {
    SetConsoleCP(CP_UTF8);        // 输入 UTF-8
    SetConsoleOutputCP(CP_UTF8);  // 输出 UTF-8
	vector<char32_t> allchar;
	vector<int> neur = { 10 , 50 , 10 };
	initc c = init(neur, 25, 0.25); //填N
	for (;;) { //或while(true)
		std::string u8input;
		cout << "训练数据（输入“<end>”结束训练）："; if (!getline(cin, u8input) || u8input == "<end>") { break; }
		bool train_eof = cin.eof();
		u32string input = mc::utf8_to_u32(u8input);
		int go_index; cout << "开始训练字符："; if (!(cin >> go_index)) { break; } cin.ignore();
		vector<vector<double>> bin;
		for (size_t i = 0; i < input.size(); i++) {
			if (findp(allchar, input[i]) == -1) { allchar.push_back(input[i]); }
			bin.push_back(vitovd(mc::bin(findp(allchar, input[i]))));
			bin[i].insert(bin[i].begin(), neur[0] - bin[i].size(), 0); //少的补上，防止越界
		}
		// 交替训练不同长度序列，缓解灾难性遗忘
		int start = std::max(1, go_index); // step=0 时 ha 为空，uw 中无法更新
		const int epochs = 100;
		const int steps_per_call = 100;
		for (int epoch = 0; epoch < epochs; epoch++) {
			for (int i = start; i < (int)input.size(); i++) {
				runc rc = NN(take(bin, i), bin[i], c, steps_per_call, i, neur, U"go", 25, 25);
				if (!rc.error.empty()) {
					for (const auto& e : rc.error) {
						std::cerr << "NN 错误：";
						for (char32_t ch : e) { std::cerr << static_cast<char>(ch); }
						std::cerr << std::endl;
					}
					break;
				}
			}
		}
	}
	
	for (;;) {
		string u8input;
		u32string input;
		cout << "用户：";
		if (!getline(cin, u8input)) { break; }
		bool user_eof = cin.eof();
		input = mc::utf8_to_u32(u8input) + U"|";
		vector<vector<double>> bin;
		for (size_t i = 0; i < input.size(); i++) {
			if (findp(allchar, input[i]) == -1) { allchar.push_back(input[i]); }
			bin.push_back(vitovd(mc::bin(findp(allchar, input[i]))));
			bin[i].resize(neur[0]);
		}
		wchar_t wch = ' ';
		int gen_count = 0;
		const int max_gen = 50;
		while (wch != ']' && gen_count < max_gen) {
			runc rc = run(c, bin, neur, (int)bin.size(), 25, 25, 0.05);
			for (size_t i = 0; i < rc.output.size(); i++) {
				cout << rc.output[i] << endl;
			}
			cout << "/" << endl;
			//rc.output = softmax(rc.output);
			rc.output = sp(rc.output);
			for (size_t i = 0; i < rc.output.size(); i++) {
				cout << rc.output[i] << endl;
			}
			bin.push_back(rc.output);
			long long idx = mc::invbin(rc.output);
			cout << idx << endl;
			// 检查网络是否学到有效模式
			bool uncertain = true;
			for (double v : rc.output) {
				if (std::abs(v - 0.5) > 0.15) { uncertain = false; break; }
			}
			if (uncertain) {
				std::cerr << "[提示] 网络输出不确定，输出占位符。" << std::endl;
				wch = '?';
			}
			else if (idx < 0 || idx >= (long long)allchar.size()) {
				std::cerr << "[提示] 生成的字符编号越界，输出占位符。" << std::endl;
				wch = '?';
			}
			else {
				wch = static_cast<wchar_t>(allchar[idx]);
			}
			//cout << "3";
			std::wcout << wch << std::endl;
			//cout << "4";
			gen_count++;
		}
		if (gen_count >= max_gen && wch != ']') {
			std::cerr << "[提示] 达到最大生成长度，停止生成。" << std::endl;
		}
		if (user_eof) { break; }
	}
}