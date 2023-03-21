# include "proxy.hpp"
# include "bind_test.cpp"
# include <random>
// # include <pybind11/pybind11.h>


void Proxy::InputAction(vector<SCALAR> action)
{
    READY = false;
}

void Proxy::InputState(vector<Vector2> state)
{
    tmp_state = state;
}

void Proxy::InputReward(SCALAR reward)
{
    tmp_reward = reward;
}

vector<SCALAR> Proxy::GetAction()
{
    vector<SCALAR> tmp_action;
    for (int i = 0; i < 3; i ++)
    {
        // control test
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dist(/* 평균 = */ 0, /* 표준 편차 = */ 1);
        double rx = (dist(gen)-.5) / 1000.0;
        
        tmp_action.push_back(rx);
    }
    READY = false;
    
    return tmp_action;
}

vector<Vector2> Proxy::GetState()
{
    return tmp_state;
}

SCALAR Proxy::GetReward()
{
    return tmp_reward;
}

void Proxy::SetReady(bool isReady)
{
    READY = isReady;
}

bool Proxy::GetReadyState()
{
    return READY;
}

// PYBIND11_MODULE(proxy, m) {
//     m.doc() = "proxy binder"; // optional module docstring

//     m.def("input_action", &Proxy::InputAction, "--");
// }
