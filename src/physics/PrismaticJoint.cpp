# include "PrismaticJoint.hpp"
# include "cmath"
# include "../Definition.hpp"

PrismaticJoint::PrismaticJoint(Body* a, Vector2 o, Body* b, Vector2 ax): Joint(a, o, b, ax)
{
    // Joint(a, offset_a, b, offset_b);
    bodyA = a;
    bodyB = b;
    offsetA = o; // offset
    offsetB = o; // offset
    axis = ax; // prismatic axis/ world
    // local axis

    m_a = bodyA->GetInverseMass();
    m_b = bodyB->GetInverseMass();
    i_a = bodyA->GetInverseInertia();
    i_b = bodyB->GetInverseInertia();
}

void PrismaticJoint::InitJoint()
{
    r_a = offsetA; //bodyA->GetPosition() + offsetA;
    r_b = offsetB; //bodyB->GetPosition() + offsetB;
    pos_a = bodyA->GetPosition();
    pos_b = bodyB->GetPosition();
    t_a = bodyA->GetRotation();
    t_b = bodyB->GetRotation();
    v_a = bodyA->GetVelocity();
    v_b = bodyB->GetVelocity();
    w_a = bodyA->GetAngularVelocity();
    w_b = bodyB->GetAngularVelocity();

    local_axis_x = {cos(t_a) * axis.x + sin(t_a) * axis.y, -sin(t_a) * axis.x + cos(t_a) * axis.y};
    local_axis_x = local_axis_x.Normalise();
    local_axis_y = local_axis_x.Cross(-1);

    mr_a = {cos(t_a) * r_a.x - sin(t_a) * r_a.y, sin(t_a) * r_a.x + cos(t_a) * r_a.y};
    mr_b = {cos(t_b) * r_b.x - sin(t_b) * r_b.y, sin(t_b) * r_b.x + cos(t_b) * r_b.y};
    Vector2 d = pos_b + mr_b - (pos_a + mr_a);

    m_perp = {cos(t_a) * r_a.x - sin(t_a) * r_a.y, sin(t_a) * r_a.x + cos(t_a) * r_a.y};
    m_s1 = d.Cross(m_perp);
    m_s2 = m_perp.Cross(r_b);

    // K is 2x2 JMJ matrix.
    K.m11 = m_a + m_b + m_s1 * m_s1 * i_a + m_s2 * m_s2 * i_b;
    K.m12 = i_a * m_s1 + i_b * m_s2;
    K.m21 = K.m12;
    K.m22 = i_a + i_b;
    if (K.m22 == 0)
    {
        K.m22 = 1;
    }

    m_impulse = {0, 0};

}

void PrismaticJoint::VelocitySolve()
{
    v_a = bodyA->GetVelocity();
    v_b = bodyB->GetVelocity();
    w_a = bodyA->GetAngularVelocity();
    w_b = bodyB->GetAngularVelocity();

    Vector2 pa_dot = bodyA->GetVelocity() + mr_a.Cross(bodyA->GetAngularVelocity());
    Vector2 pb_dot = bodyB->GetVelocity() + mr_b.Cross(bodyB->GetAngularVelocity());
    Vector2 C_dot; // = J * V
    C_dot.x = m_perp.Dot(v_b - v_a) + m_s2 * w_b - m_s1 * w_a; // = J * V
    C_dot.y = w_b - w_a;
    Vector2 df = K.Solve(-C_dot);
    m_impulse += df;

    Vector2 P = m_perp * df.x;
    double LA = df.x * m_s1 + df.y;
    double LB = df.x * m_s2 + df.y;

    bodyA->AddVelocity(-P * m_a);
    bodyA->AddAngularVelocity(-LA * i_a);
    bodyB->AddVelocity(P * m_b);
    bodyB->AddAngularVelocity(LB * i_b);
    // PrintVector("joint impulse", impulse);

}

void PrismaticJoint::PositionSolve()
{
    pos_a = bodyA->GetPosition();
    pos_b = bodyB->GetPosition();
    t_a = bodyA->GetRotation();
    t_b = bodyB->GetRotation();
    
    r_a = {cos(t_a) * r_a.x - sin(t_a) * r_a.y, sin(t_a) * r_a.x + cos(t_a) * r_a.y};
    r_b = {cos(t_b) * r_b.x - sin(t_b) * r_b.y, sin(t_b) * r_b.x + cos(t_b) * r_b.y};
    Vector2 d = pos_b + r_b - (pos_a + r_a);

    Vector2 ax = {cos(t_a) * local_axis_x.x - sin(t_a) * local_axis_x.y, sin(t_a) * local_axis_x.x + cos(t_a) * local_axis_x.y};
    double a1 = (d + r_a).Cross(local_axis_x);
    double a2 = r_b.Cross(ax);
    Vector2 perp = {cos(t_a) * local_axis_y.x - sin(t_a) * local_axis_y.y, sin(t_a) * local_axis_y.x + cos(t_a) * local_axis_y.y};

    double s1 = (d + r_a).Cross(perp);
    double s2 = r_b.Cross(perp);


    ////
    
    // Vector2 C = pos_b + r_b - (pos_a + r_a);
    // double pos_error = C.GetLength();

    // K.m11 = m_a + m_b + i_a * r_a.y * r_a.y + i_b * r_b.y * r_b.y;
    // K.m12 = -i_a * r_a.x * r_a.y -i_b * r_b.x * r_b.y;
    // K.m21 = K.m12;
    // K.m22 = m_a + m_b + i_a * r_a.x * r_a.x + i_b * r_b.x * r_b.x;
    // Vector2 impulse = -K.Solve(C);

    // bodyA->AddTranslation(-impulse * m_a, -r_a.Cross(impulse) * i_a);
    // bodyB->AddTranslation(impulse * m_b, r_b.Cross(impulse) * i_b);

    // PrintVector("position correction", pos_a);

}

void PrismaticJoint::ApplyJointImpulse()
{
    bodyA->AddJointImpulse(v_a, w_a);
    bodyB->AddJointImpulse(v_b, w_b);
}