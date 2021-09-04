# include "Collision.hpp"
# include <cmath>

Collision::Collision(Body* b1, Body* b2)
{
    this->b1 = b1;
    this->b2 = b2;

    bgt = .2;
    slop = .0;
    max_correction = .5;
    separation = 0;
}

void Collision::FindCollisioninfo(Simplex simplex)
{
    // EPA algorithm for find collision normal, penetration depth.
    while (true)
    {
        Edge e = FindClosetEdge(simplex);
        Vector2 p = SupportFunction(b1->GetCollider()->GetVertices(), b2->GetCollider()->GetVertices(), e.normal);
        double d = p.Dot(e.normal);
        if (d - e.distance < TOLERANCE)
        {
            collisionNormal = e.normal;
            penetrationDepth = d;
            // cout << "> collision info: " << collisionNormal.x << ", " << collisionNormal.y << ", " << penetrationDepth << endl;            
            break;
        }
        else
        {
            simplex.Insert(p, e.index);
        }
    }
}

void Collision::FindManifolds()
{
    // 
    flip = false;
    Edge e1 = b1->GetCollider()->FindBestEdge(collisionNormal);
    Edge e2 = b2->GetCollider()->FindBestEdge(-collisionNormal);

    // cout << "> find best edge" << endl;

    Edge ref, inc;

    if (abs(e1.GetVector().Dot(collisionNormal) <= abs(e2.GetVector().Dot(collisionNormal))))
    {
        ref = e1;
        inc = e2;
    }
    else
    {
        ref = e2;
        inc = e1;
        flip = true;
        // cout << "flipped!" << endl;
    }

    Vector2 normalizedRef = ref.GetVector().Normalise();
    double offset1 = normalizedRef.Dot(ref.a);
    ClippedPoints cp = Clip(inc.a, inc.b, normalizedRef, offset1);

    if (cp.cPoints.size() < 2)
    {
        // cout << "less than 2 point (1)" << endl;
        return;
    }
    
    double offset2 = normalizedRef.Dot(ref.b);

    // cout << "> offset2, " << offset2 << endl;
    
    ClippedPoints cp2 = Clip(cp.cPoints[0], cp.cPoints[1], -normalizedRef, -offset2);
    if (cp2.cPoints.size() < 2)
    {
        // cout << "> less than 2 point (2)" << endl;
        return;
    }

    // for (int i = 0; i < 2; i++)
    //     cout << "> points: " << cp2.cPoints[i].x << ", " << cp2.cPoints[i].y << endl;

    Vector2 refNormal = ref.GetVector().Cross(-1.0);
    if (flip)
    {
        refNormal = -refNormal;
    }
    double max = refNormal.Dot(ref.farthest);
    // cout << "> max vertex: " << max << endl;

    // cout << "point count_1: " << cp2.cPoints.size() << endl;
    if (refNormal.Dot(cp2.cPoints[0]) - max < 0.0)
    {
        // cout << "> erase 1, dot: " << refNormal.Dot(cp2.cPoints[0]) << endl;

        if (refNormal.Dot(cp2.cPoints[1]) - max < 0.0)
        {
            // PrintVector("ref norm", refNormal);
            // PrintVector("erase 1, 2", cp2.cPoints[1]);
            // PrintScalar("max", max);
            // cout << "> erase 1, 2" << endl;
            // cp2.cPoints.erase(cp2.cPoints.begin()); // 충돌지점 다 사라지는 거 방지하는 임시방편.
            cp2.cPoints.erase(cp2.cPoints.begin());
        }
        else
        {
            // cout << "erase 1" << endl;
            cp2.cPoints.erase(cp2.cPoints.begin());
        }
    }
    else
    {
        if (refNormal.Dot(cp2.cPoints[1]) - max < 0.0)
        {
            // cout << "> erase 2" << endl;
            cp2.cPoints.erase(cp2.cPoints.begin() + 1);

        }
        
    }

    manifolds = cp2;

    for (int i = 0; i < manifolds.cPoints.size(); i ++)
    {
        // cout << "> manifold: " << manifolds.cPoints[i].x << ", " << manifolds.cPoints[i].y << endl;
    }

}

ClippedPoints Collision::Clip(Vector2 p1, Vector2 p2, Vector2 n, double o)
{
    ClippedPoints cp;
    double d1 = n.Dot(p1) - o;
    double d2 = n.Dot(p2) - o;

    if (d1 > 0.0)
    {
        cp.cPoints.push_back(p1);
    }
    if (d2 > 0.0)
    {
        cp.cPoints.push_back(p2);
    }

    if (d1 * d2 < 0.0)
    {
        Vector2 e = p2 - p1;
        double u = d1 / (d1 - d2);
        e *= u;
        e += p1;
        cp.cPoints.push_back(e);
    }

    return cp;
}

void Collision::InitSolver()
{
    m_a = b1->GetInverseMass();
    m_b = b2->GetInverseMass();
    i_a = b1->GetInverseInertia();
    i_b = b2->GetInverseInertia();
    
    // temporal attributes
    pos_a = b1->GetPosition();
    pos_b = b2->GetPosition();
    angle_a = b1->GetRotation();
    angle_b = b2->GetRotation();
    v_a = b1->GetVelocity();
    v_b = b2->GetVelocity();
    w_a = b1->GetAngularVelocity();
    w_b = b2->GetAngularVelocity();

    // contact point from center
    // cout << "> manifold count - " << manifolds.cPoints.size() << endl;
    if (manifolds.cPoints.size() == 0)
        return;
    r_a = manifolds.cPoints[0] - pos_a;
    r_b = manifolds.cPoints[0] - pos_b;

    // normal to <R> vector
    rn_a = r_a.Cross(collisionNormal);
    rn_b = r_b.Cross(collisionNormal);

    n_impulse = 0;
    t_impulse = 0;
    
    k_normal = m_a + m_b + i_a * rn_a * rn_a + i_b * rn_b * rn_b;
    m_normal = k_normal > 0 ? 1 / k_normal : 0;
    tangent = collisionNormal.Cross(-1);
    rt_a = r_a.Cross(tangent);
    rt_b = r_b.Cross(tangent);
    
    k_tangent = m_a + m_b + i_a * rt_a * rt_a + i_b * rt_b * rt_b;
    m_tangent = k_tangent > 0 ? 1 / k_tangent : 0;

    // for restitution
    v_rel = collisionNormal.Dot(v_b + r_b.Cross(w_b) - (v_a + r_a.Cross(w_a)));
    e = .0;
    // PrintScalar("relative vel", v_rel);
    bias = 0;
    if (v_rel < -1.5)
        bias = -e * v_rel;

    // for friction
    friction = .0;
    // PrintScalar("bias", bias);

    // for position solve
    separation = -penetrationDepth;

    for (int i = 0; i < manifolds.cPoints.size(); i ++)
    {
        // PrintVector("manifold", manifolds.cPoints[i]);
    }
}


void Collision::VelocitySolve()
{
    // collision solve using constraint based method.

    // solve tangent constraint for friction.
    // 
    v_a = b1->GetVelocity();
    v_b = b2->GetVelocity();
    w_a = b1->GetAngularVelocity();
    w_b = b2->GetAngularVelocity();
    
    Vector2 dv = v_b + r_b.Cross(w_b) - (v_a + r_a.Cross(w_a));
    SCALAR v_t = dv.Dot(tangent);
    SCALAR lambda = m_tangent * -v_t;
    SCALAR max_friction = friction * n_impulse;
    double new_impulse = Clamp(t_impulse + lambda, -max_friction, max_friction);
    lambda = new_impulse - t_impulse;
    t_impulse = new_impulse;

    Vector2 P = tangent * lambda;
    b1->AddVelocity(-P * m_a);
    b1->AddAngularVelocity(-r_a.Cross(P) * i_a);
    b2->AddVelocity(P * m_b);
    b2->AddAngularVelocity(r_b.Cross(P) * i_b);

    // solve normal constraint for restitution, no penetration.
    SCALAR v_n = collisionNormal.Dot(v_b + r_b.Cross(w_b) - (v_a + r_a.Cross(w_a)));
    // SCALAR v_n = dv.Dot(collisionNormal);
    lambda = -m_normal * (v_n - bias);
    // PrintScalar("pure lambda", lambda);
    new_impulse = fmax(n_impulse + lambda, 0);
    lambda = new_impulse - n_impulse;
    n_impulse = new_impulse;
    // PrintScalar("n_impulse", n_impulse);
    
    P = collisionNormal * lambda;
    b1->AddVelocity(-P * m_a);
    b1->AddAngularVelocity(-r_a.Cross(P) * i_a);
    b2->AddVelocity(P * m_b);
    b2->AddAngularVelocity(r_b.Cross(P) * i_b);

    // PrintVector("impulse", P);
    // PrintVector("v", b2->GetVelocity());
    // PrintScalar("w", b2->GetAngularVelocity());

}

void Collision::PositionSolve()
{
    double min_separation = 0;
    Vector2 pos_a = b1->GetPosition();
    Vector2 pos_b = b2->GetPosition();
    SCALAR t_a = b1->GetRotation();
    SCALAR t_b = b2->GetRotation();
    // double bgt = .5;
    // double slop = .01;
    // double max_correction = .2;

    // solve normal constraint.
    if (manifolds.cPoints.size() == 0)
        return;

    Vector2 r_a = manifolds.cPoints[0] - pos_a;
    Vector2 r_b = manifolds.cPoints[0] - pos_b;

    // min separation???
    // separation = -penetrationDepth;
    // min_separation = min(min_separation, separation);

    double C = Clamp(bgt * (separation + slop), -max_correction, 0);
    SCALAR rn_a = r_a.Cross(collisionNormal);
    SCALAR rn_b = r_b.Cross(collisionNormal);
    double K = m_a + m_b + i_a * rn_a * rn_a + i_b * rn_b * rn_b;
    SCALAR impulse = K > 0 ? -C / K : 0;
    Vector2 P = collisionNormal * impulse;
    // PrintScalar("correction", C);
    // PrintScalar("K", K);

    b1->AddPosition(-P * m_a);
    b1->AddRotation(-r_a.Cross(P) * i_a);
    b2->AddPosition(P * m_b);
    b2->AddRotation(r_b.Cross(P) * i_b);
    // b2->AddImpulseAt(P, manifolds.cPoints[0]);
    // PrintVector("correction dist", P * m_b);
    // PrintVector("corrected pos", pos_b);
}

void Collision::Push()
{
    // penetration 이 너무 작지 않으면 그 깊이만큼 튕겨내줌.
    if (penetrationDepth > 0.0001)
    {
        Vector2 push = collisionNormal * penetrationDepth * 1;
        b2->AddPosition(push);
        // PrintVector("vel on push", b2->GetVelocity());
    }
}

// deprecated naive velocity solve.
// void Collision::VelocitySolve2()
// {
//     // PrintVector("before velocity", b2->GetVelocity());
//     // PrintVector("normal", b2->GetVelocity());

//     SCALAR e = .5;
//     SCALAR mu = .02;
//     Vector2 relative = b1->GetVelocity() - b2->GetVelocity();
//     SCALAR normalVelocity = collisionNormal.Dot(relative);
//     SCALAR impulse = (1 + e) * normalVelocity / (collisionNormal.Dot(collisionNormal) * (b1->GetInverseMass() + b2->GetInverseMass()));
    
//     // Vector2 relative = b1->GetVelocity() - b2->GetVelocity();
//     // SCALAR normalVelocity = collisionNormal.Dot(relative);
//     // SCALAR impulse = (1 + e) * normalVelocity / (collisionNormal.Dot(collisionNormal) * (b1->GetInverseMass() + b2->GetInverseMass()));
//     Vector2 tangential = collisionNormal.Cross(-1);

//     Vector2 norm = collisionNormal.Normalise();
//     // PrintVector("norm", norm);

//     if (flip)
//     {
//         // cout << "flipped!!!!!!" << endl;
//     }
    
//     for (int i = 0; i < manifolds.cPoints.size(); i ++)
//     {
//         // PrintVector("manifold", manifolds.cPoints[i]);
//     }
    
//     // impulse
//     if(manifolds.cPoints.size() > 0)
//     {
//         if (b1->GetType() != DYNAMIC)
//         {
//             // 둘 다 dynamic 이 아님.
//             if (b2->GetType() != DYNAMIC)
//             {
//                 ;
//             }
//             // b2 만 dynamic
//             else
//             {
//                 // PrintScalar("impulse 2", impulse);
//                 // penetration 이 너무 작지 않으면 그 깊이만큼 튕겨내줌.
//                 if (penetrationDepth > 0.0001)
//                 {
//                     Vector2 push = collisionNormal * penetrationDepth * .8;
//                     b2->AddPosition(push);
//                 }

//                 Vector2 friction = tangential * impulse * mu;
//                 if (b2->GetVelocity().Dot(tangential) > 0)
//                     friction *= -1;
//                 b2->AddImpulseAt(norm * impulse + friction, manifolds.cPoints[0]);
//                 b2->SetAngular(b2->GetAngularVelocity() + b2->GetAngularVelocity() * -.1);
//                 // PrintVector("b2 velocity", b2->GetVelocity());
//                 // // 속도가 너무 작을 때 impulse 를 주지않고 정지시킴.
//                 // if (b2->GetVelocity().GetLength() < 0.3)
//                 // {
//                 //     Vector2 friction = norm.Cross(-1) * impulse * mu;
//                 //     PrintScalar("no impulse", b2->GetVelocity().GetLength());
//                 //     PrintScalar("no rot", b2->GetAngularVelocity());
//                 //     b2->SetVel(b2->GetVelocity() + friction);
//                 //     // b2->SetAngular(b2->GetAngularVelocity() * .95);
                    
//                 //     // if (abs(b2->GetAngularVelocity()) < 1)
//                 //     // {
//                 //     //     b2->SetAngular(0);
//                 //     // }

//                 // }
//             }
//         }
//         else
//         {
//             // b1 만 dynamic
//             if (b2->GetType() != DYNAMIC)
//             {
//                 // PrintScalar("impulse 1", impulse);
//                 // penetration 이 너무 작지 않으면 그 깊이만큼 튕겨내줌.
//                 if (penetrationDepth > 0.0001)
//                 {
//                     Vector2 push = collisionNormal * penetrationDepth * 1;
//                     b1->SetPosition(b1->GetPosition() - push);
//                 }

//                 Vector2 friction = tangential * impulse * mu;
//                 if (b1->GetVelocity().Dot(tangential) > 0)
//                     friction *= -1;
//                 b1->AddImpulseAt(-norm * impulse + friction, manifolds.cPoints[0]);
//                 b1->SetAngular(b1->GetAngularVelocity() + b1->GetAngularVelocity() * -.1);
//                 // PrintVector("b1 velocity", b1->GetVelocity());
//                 // 속도가 너무 작을 때 impulse 를 주지않고 정지시킴.
//                 // if (b1->GetVelocity().GetLength() < 0.3)
//                 // {
//                 //     // PrintScalar("no impulse", b2->GetVelocity().GetLength());
//                 //     // PrintScalar("no rot", b2->GetAngularVelocity());
//                 //     b1->SetVel(b2->GetVelocity() * .9);
//                 //     b1->SetAngular(b2->GetAngularVelocity() * .9);
                    
//                 //     SCALAR tmp = abs(b1->GetVelocity().Dot(collisionNormal.Cross(-1)));
//                 //     PrintScalar("range", tmp);

//                 //     if (abs(b1->GetAngularVelocity()) < 1)
//                 //     {
//                 //         b1->SetAngular(0);
//                 //     }

//                 // }
//             }

//             // 둘 다 dynamic
//             else
//             {
//                 // penetration 이 너무 작지 않으면 그 깊이만큼 튕겨내줌.
//                 if (penetrationDepth > 0.0001)
//                 {
//                     Vector2 push = collisionNormal * penetrationDepth * 1.0;
//                     b1->SetPosition(b1->GetPosition() - push);
//                     b2->SetPosition(b2->GetPosition() + push);
//                     // PrintVector("impulse", impulse_2);
//                 }

//                 b1->AddImpulseAt(-norm * impulse, manifolds.cPoints[0]);
//                 b2->AddImpulseAt(norm * impulse, manifolds.cPoints[0]);
//             }
//         }
//     }
//     else
//     {
//         // cout << "no manifold" << endl;
//     }
//     // PrintVector("after velocity", b2->GetVelocity());

// }