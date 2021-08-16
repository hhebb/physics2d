# ifndef REVOLUTE_JOINT
# define REVOLUTE_JOINT

# include "Joint.hpp"

class RevoluteJoint: public Joint
{
private:
    Matrix2x2 K;
    SCALAR m_a;
    SCALAR m_b;
    SCALAR i_a;
    SCALAR i_b;

    Vector2 r_a;
    Vector2 r_b;
    SCALAR t_a;
    SCALAR t_b;
    Vector2 mr_a;
    Vector2 mr_b;

    Vector2 v_a;
    Vector2 v_b;
    SCALAR w_a;
    SCALAR w_b;

    Vector2 pos_a;
    Vector2 pos_b;
    SCALAR angle_a;
    SCALAR angle_b;


public:
    RevoluteJoint(Body* a, Vector2 offset_a, Body* b, Vector2 offset_b);
    void InitJoint() override;
    void VelocitySolve() override;
    void PositionSolve() override;
    void ApplyJointImpulse() override;

};

# endif