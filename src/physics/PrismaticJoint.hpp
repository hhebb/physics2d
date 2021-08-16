# ifndef PRISMATIC_JOINT
# define PRISMATIC_JOINT

# include "Joint.hpp"

class PrismaticJoint: public Joint
{
private:
    Vector2 anchor;
    Vector2 axis;
    Vector2 local_axis_x;
    Vector2 local_axis_y;

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

    Vector2 m_perp;
    double m_s1;
    double m_s2;
    Vector2 m_impulse;

public:
    PrismaticJoint(Body* a, Vector2 o, Body* b, Vector2 ax);
    void InitJoint() override;
    void VelocitySolve() override;
    void PositionSolve() override;
    void ApplyJointImpulse() override;

};

# endif