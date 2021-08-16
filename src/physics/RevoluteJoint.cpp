# include "RevoluteJoint.hpp"
# include "cmath"
# include "../Definition.hpp"

RevoluteJoint::RevoluteJoint(Body* a, Vector2 offset_a, Body* b, Vector2 offset_b): Joint(a, offset_a, b, offset_b)
{
    // Joint(a, offset_a, b, offset_b);
    bodyA = a;
    bodyB = b;
    offsetA = offset_a;
    offsetB = offset_b;

    m_a = bodyA->GetInverseMass();
    m_b = bodyB->GetInverseMass();
    i_a = bodyA->GetInverseInertia();
    i_b = bodyB->GetInverseInertia();
}

void RevoluteJoint::InitJoint()
{
    r_a = offsetA;
    r_b = offsetB;
    t_a = bodyA->GetRotation();
    t_b = bodyB->GetRotation();
    v_a = bodyA->GetVelocity();
    v_b = bodyB->GetVelocity();
    w_a = bodyA->GetAngularVelocity();
    w_b = bodyB->GetAngularVelocity();

    mr_a = r_a.SimpleRotate(t_a); //{cos(t_a) * r_a.x - sin(t_a) * r_a.y, sin(t_a) * r_a.x + cos(t_a) * r_a.y};
    mr_b = r_b.SimpleRotate(t_b); //{cos(t_b) * r_b.x - sin(t_b) * r_b.y, sin(t_b) * r_b.x + cos(t_b) * r_b.y};

    // K is 2x2 JMJ matrix.
    K.m11 = m_a + m_b + mr_a.y * mr_a.y * i_a + mr_b.y * mr_b.y * i_b;
    K.m12 = -mr_a.y * mr_a.x * i_a - mr_b.y * mr_b.x * i_b;
    K.m21 = K.m12;
    K.m22 = m_a + m_b + mr_a.x * mr_a.x * i_a + mr_b.x * mr_b.x * i_b;

}

void RevoluteJoint::VelocitySolve()
{
    Vector2 pa_dot = bodyA->GetVelocity() + mr_a.Cross(bodyA->GetAngularVelocity());
    Vector2 pb_dot = bodyB->GetVelocity() + mr_b.Cross(bodyB->GetAngularVelocity());
    Vector2 C_dot = pb_dot - pa_dot; // = J * V
    Vector2 impulse = K.Solve(-C_dot);

    bodyA->AddVelocity(-impulse * m_a);
    bodyA->AddAngularVelocity(-mr_a.Cross(impulse) * i_a);
    bodyB->AddVelocity(impulse * m_b);
    bodyB->AddAngularVelocity(mr_b.Cross(impulse) * i_b);
    // PrintVector("joint impulse", impulse);

}

void RevoluteJoint::PositionSolve()
{
    pos_a = bodyA->GetPosition();
    pos_b = bodyB->GetPosition();
    angle_a = bodyA->GetRotation();
    angle_b = bodyB->GetRotation();

    t_a = bodyA->GetRotation();
    t_b = bodyB->GetRotation();
    r_a = r_a.SimpleRotate(t_a);
    r_b = r_b.SimpleRotate(t_b);
    Vector2 C = pos_b + r_b - (pos_a + r_a);
    double pos_error = C.GetLength();

    K.m11 = m_a + m_b + i_a * r_a.y * r_a.y + i_b * r_b.y * r_b.y;
    K.m12 = -i_a * r_a.x * r_a.y -i_b * r_b.x * r_b.y;
    K.m21 = K.m12;
    K.m22 = m_a + m_b + i_a * r_a.x * r_a.x + i_b * r_b.x * r_b.x;
    Vector2 impulse = -K.Solve(C);

    bodyA->AddPosition(-impulse * m_a);
    bodyA->AddRotation(-r_a.Cross(impulse) * i_a);
    bodyB->AddPosition(impulse * m_b);
    bodyB->AddRotation(r_b.Cross(impulse) * i_b);

    // PrintVector("position correction", pos_a);

}

void RevoluteJoint::ApplyJointImpulse()
{
    bodyA->AddJointImpulse(v_a, w_a);
    bodyB->AddJointImpulse(v_b, w_b);
}