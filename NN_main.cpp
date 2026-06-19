#include "init.hpp"
#include "run.hpp"
#include "uw.hpp"
#include "Softmax.hpp"
#define init_lr 0.1
#define PI 3.1415926

inline int NN(vector<vector<double>> input, vector<double> tv, initc c,
size_t _step_ = 1000, vector<int> neur = { 5, 20, 5 }, string m = "go") {
    //string m;
    //cin >> m;
    if (m == "go") {
        //vector<int> neur = { 5, 20, 5 };
        /*vector<vector<double>> input(5, {0.0, 0.0, 0.0, 0.0, 0.0});input[0][0] = 1.0;
        vector<double> tv = { 0.0, 0.1, 0.0, 0.0, 0.0 };*/
        //initc c = init(neur, 10, 0.25);
        vector<double> yes;
        //cout << "[system]init[run good]\n" << endl;
        /*cin.ignore();
        cin >> _step_;*/
        double lr = init_lr;
        int T = 200;
        double se = 0;
        for (size_t i = 0; i < _step_; i++) {
            runc rc = run(c.iw, c.hw, c.b, c.ow, input, neur, 5, 10, 10, 0.05);
            /*cout << "[system]run[run good]\n";
            cout << "[output]run{" << endl;
            for (size_t i = 0; i < rc.output.size(); i++) { cout << "    " << i << ">" << rc.output[i] << "\n"; }
            cout << "}\nrun[output]/\n";*/
            vector<double> opsm = softmax(rc.output);
            /*cout << "[Softmax(output)]run{" << endl;
            for (size_t i = 0; i < opsm.size(); i++) { cout << "    " << i << ">" << opsm[i] << "\n"; }
            cout << "}\nrun[Softmax(output)]/\n";*/
            double ny = uw(c.iw, c.hw, c.b, c.ow, input, rc.ha, opsm, neur, 5, tv, 10, 10, lr);
            yes.push_back(ny);
            se += ny;
            double t = _step_ % T;
            lr = 1e-3 * 0.5 * (1.0 + std::cos(PI * t / T));
            lr = std::max(lr, 1e-4);
        }
        /*for (size_t i = 0; i < yes.size(); i++) { cout << yes[i] << endl; }
        cout << lr << endl;
        cout << se / _step_ << endl;
        cout << "\n";
        for (size_t i = 0; i < c.b.size(); i++) { cout << c.b[i] << endl; }*/
    }
    else if (m == "end") { return 0; }
    else { return 1; }
    return 0;
}