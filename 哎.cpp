#include "init.hpp"
#include "run.hpp"
#include "uw.hpp"
#include "Softmax.hpp"

int main(){
    vector<int> neur = { 5, 20, 1 };
    vector<vector<double>> input(5, { 0.0, 0.0, 0.0, 0.0, 0.0 });input[0][0] = 1.0;
    vector<double> tv = { 1 };
    initc c = init(neur, 10);
    cout << "[system]init[run good]\n" << endl;
    size_t _step_ = 100000000000;
    double lr = 0.1;
    double s = 0.7;
    for (size_t i = 0; i < _step_; i++) {
        runc rc = run(c.iw, c.hw, c.b, c.ow, input, neur, 5, 10, 10, 0.1);
        cout << "[system]run[run good]\n";
        cout << "[output]run{" << endl;
        for (size_t i = 0; i < rc.output.size(); i++) { cout << "    " << i << ">" << rc.output[i] << "\n"; }
        cout << "}\n";
        vector<double> opsm = softmax(rc.output);
        cout << "[Softmax(output)]run{" << endl;
        for (size_t i = 0; i < opsm.size(); i++) { cout << "    " << i << ">" << opsm[i] << "\n"; }
        cout << "}\n";
        uw(c.iw, c.hw, c.b, c.ow, input, rc.ha, rc.output, neur, 5, tv, 10, 10, lr);
        lr *= s;
    }
    return 0;
}