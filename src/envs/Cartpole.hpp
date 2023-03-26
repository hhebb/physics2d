# ifndef CARTPOLE
# define CARTPOLE

# include "BaseEnv.hpp"

class Cartpole: public BaseEnv
{
private:
    SCALAR reward;
public:
    SCALAR GetReward(vector<SCALAR> state) override
    {
        // state: pole angle, horizontal cart position.
        SCALAR angle = state[0];
        SCALAR pos = state[1];

        if (angle > 60.0 or pos > 1.0 or pos < -1.0)
        {
            // episode done
            reward = -200.0;
        }
        else
        {
            reward = 1.0;
        }

        return reward;
    }

    bool IsDone(vector<SCALAR> state) override
    {
        bool done = false;
        SCALAR angle = state[0];
        SCALAR pos = state[1];

        if (angle > 60.0 or pos > 1.0 or pos < -1.0)
        {
            done = true;
        }
        else
        {
            done = false;
        }

        return done;
    }
};

# endif