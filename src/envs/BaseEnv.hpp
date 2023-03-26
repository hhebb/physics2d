# ifndef BASE_ENV
# define BASE_ENV
# include "../Definition.hpp"

// # include "Cartpole.hpp"

class BaseEnv
{
private:
    SCALAR reward;
public:
    virtual SCALAR GetReward(vector<SCALAR> state) = 0;
    virtual bool IsDone(vector<SCALAR> state) = 0;
};

# endif