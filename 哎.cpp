#include "init.hpp"
#include "run.hpp"

int main(){
    vector<int> neur = { 10, 20, 10 };
    vector<double> input = { 1.0, 0.0, 
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
    0.0 };
    initc c = init(neur);
    cout << "[system]init[run good]" << endl;
    runc rc = run(c.iw, c.hw, c.b, c.ow, input, neur, 5);
    cout << "[system]run[run good]" << endl;
    return 0;
}