#include "NN.hpp"
#include "my_class.hpp"
#include <windows.h>

static int findp(vector<char32_t> a, char32_t b) {
	int ret = -1;
	for (int i = 0; i < a.size(); i++) { if (a[i] == b) { ret = (int)i; break; } }
	return ret;
}

static vector<vector<double>> take(const vector<vector<double>> a, int b) {
	vector<vector<double>> ret(b);
	for (size_t i = 0; i < b; i++) { ret[i] = a[i]; }
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
	vector<int> neur = { 2 , 10 , 2 };
	initc c = init(neur, 5, 0.25);
	std::string u8input;
	getline(cin, u8input);
    u32string input = mc::utf8_to_u32(u8input);
	vector<vector<double>> bin;
	for (size_t i = 0; i < input.size(); i++) {
		if (findp(allchar, input[i]) == -1) { allchar.push_back(input[i]); }
		bin.push_back(vitovd(mc::bin(findp(allchar, input[i]))));
		bin[i].resize(neur[0]); //少的补上，防止越界
	}
	//训练：
	for (int i = 1; i < input.size(); i++) {
		NN(take(bin, i), bin[i], c, 5, i, neur, U"go", 5, 5);
		//cout << "1" << endl;
	}
}