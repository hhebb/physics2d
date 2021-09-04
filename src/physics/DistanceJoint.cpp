# include "DistanceJoint.hpp"

DistanceJoint::DistanceJoint(Body* a, Vector2 offset_a, Body* b, Vector2 offset_b): Joint(a, offset_a, b, offset_b)
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

    d = (offset_a - offset_a).GetLength();
}

void DistanceJoint::InitJoint()
{
    pos_a = bodyA->GetPosition();
    pos_b = bodyB->GetPosition();
    r_a = offsetA;
    r_b = offsetB;
    t_a = bodyA->GetRotation();
    t_b = bodyB->GetRotation();
    v_a = bodyA->GetVelocity();
    v_b = bodyB->GetVelocity();
    w_a = bodyA->GetAngularVelocity();
    w_b = bodyB->GetAngularVelocity();

    mr_a = r_a.SimpleRotate(t_a);
    mr_b = r_b.SimpleRotate(t_b);
    m_u = pos_b + mr_b - (pos_a + mr_a);
    cr_au = mr_a.Cross(m_u);
    cr_bu = mr_b.Cross(m_u);
    SCALAR inv_tmp_m = m_a + i_a * cr_au * cr_au + m_b + i_b * cr_bu * cr_bu;
    inv_m = inv_tmp_m != 0 ? 1 / inv_tmp_m : 0;
    double m_gamma = 0;
    double bias = 0;
    SCALAR m_soft = inv_m;
    m_impulse = 0;

}

void DistanceJoint::VelocitySolve()
{
    v_a = bodyA->GetVelocity();
    v_b = bodyB->GetVelocity();
    w_a = bodyA->GetAngularVelocity();
    w_b = bodyB->GetAngularVelocity();

    Vector2 vp_a = bodyA->GetVelocity() + mr_a.Cross(w_a);
    Vector2 vp_b = bodyB->GetVelocity() + mr_b.Cross(w_b);
    SCALAR C_dot = m_u.Dot(vp_b - vp_a); // = J * V
    SCALAR impulse = -inv_m * C_dot;
    m_impulse += impulse;

    Vector2 P = m_u * impulse;

    bodyA->AddVelocity(-P * m_a);
    bodyA->AddAngularVelocity(-mr_a.Cross(P) * i_a);
    bodyB->AddVelocity(P * m_b);
    bodyB->AddAngularVelocity(mr_b.Cross(P) * i_b);

    // PrintVector("p impulse", P);
}

void DistanceJoint::PositionSolve()
{
    pos_a = bodyA->GetPosition();
    pos_b = bodyB->GetPosition();
    t_a = bodyA->GetRotation();
    t_b = bodyB->GetRotation();

    r_a = r_a.SimpleRotate(t_a);
    r_b = r_b.SimpleRotate(t_b);
    Vector2 u = pos_b + r_b - (pos_a + r_a);
    SCALAR length = u.GetLength();
    double C = length - d;

    SCALAR impulse = -inv_m * C;
    Vector2 P = u * impulse;
    bodyA->AddPosition(-P * m_a);
    bodyA->AddRotation(- r_a.Cross(P) * i_a);
    bodyB->AddPosition(P * m_b);
    bodyB->AddRotation(r_b.Cross(P) * i_b);
    // PrintVector("dist impulse", P);
    // PrintVector("angle", r_b);
    // PrintScalar("m", m_b);
}


void DistanceJoint::ApplyJointImpulse()
{}