#include "init.hpp"
#include "run.hpp"
#include "Softmax.hpp"

int main(){
    vector<int> neur = { 5, 20, 1 };
    vector<vector<double>> input(5, { 0.0, 0.0, 0.0, 0.0, 0.0 });input[0][0] = 1.0;
    initc c = init(neur, 10);
    cout << "[system]init[run good]\n" << endl;
    runc rc = run(c.iw, c.hw, c.b, c.ow, input, neur, 5, 10, 10, 0.1);
    cout << "[system]run[run good]\n";
    cout << "[output]run{" << endl;
    for (size_t i = 0; i < rc.output.size(); i++) {cout << "    " << i << ">" << rc.output[i] << "\n"; }
    cout << "}\n";
    vector<double> opsm = softmax(rc.output);
    cout << "[Softmax(output)]run{" << endl;
    for (size_t i = 0; i < opsm.size(); i++) { cout << "    " << i << ">" << opsm[i] << "\n"; }
    cout << "}\n";

    return 0;
}