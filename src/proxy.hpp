# ifndef PROXY
# define PROXY

# include "Definition.hpp"

class Proxy
{
private:
    bool READY = false;
    vector<SCALAR> tmp_action;
    vector<Vector2> tmp_state;
    SCALAR tmp_reward;
public:
    void InputAction(vector<SCALAR> action);
    void InputState(vector<Vector2> state);
    void InputReward(SCALAR reward);

    vector<SCALAR> GetAction();
    vector<Vector2> GetState();
    SCALAR GetReward();
    void SetReady(bool isReady);
    bool GetReadyState();

};

# endif