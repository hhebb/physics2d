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
    }

    Vector2 normalizedRef = ref.GetVector().Normalise();
    double offset1 = normalizedRef.Dot(ref.a);
    ClippedPoints cp = Clip(inc.a, inc.b, normalizedRef, offset1);

    if (cp.cPoints.size() < 2)
    {
        return;
    }
    
    double offset2 = normalizedRef.Dot(ref.b);

    ClippedPoints cp2 = Clip(cp.cPoints[0].pos, cp.cPoints[1].pos, -normalizedRef, -offset2);
    if (cp2.cPoints.size() < 2)
    {
        return;
    }

    Vector2 refNormal = ref.GetVector().Cross(-1.0);
    if (flip)
    {
        refNormal = -refNormal;
    }
    double max = refNormal.Dot(ref.farthest);

    if (refNormal.Dot(cp2.cPoints[0].pos) - max < 0.0)
    {
        if (refNormal.Dot(cp2.cPoints[1].pos) - max < 0.0)
        {
            cp2.cPoints.erase(cp2.cPoints.begin());
        }
        else
        {
            cp2.cPoints.erase(cp2.cPoints.begin());
        }
    }
    else
    {
        if (refNormal.Dot(cp2.cPoints[1].pos) - max < 0.0)
        {
            cp2.cPoints.erase(cp2.cPoints.begin() + 1);
        }
        
    }

    manifolds = cp2;
}

ClippedPoints Collision::Clip(Vector2 p1, Vector2 p2, Vector2 n, double o)
{
    ClippedPoints cp;
    double d1 = n.Dot(p1) - o;
    double d2 = n.Dot(p2) - o;

    if (d1 > 0.0)
    {
        ContactPoint pt;
        pt.pos = p1;
        cp.cPoints.push_back(pt);
    }
    if (d2 > 0.0)
    {
        ContactPoint pt;
        pt.pos = p2;
        cp.cPoints.push_back(pt);
    }

    if (d1 * d2 < 0.0)
    {
        Vector2 e = p2 - p1;
        double u = d1 / (d1 - d2);
        e *= u;
        e += p1;
        
        ContactPoint pt;
        pt.pos = e;
        cp.cPoints.push_back(pt);
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

    for (vector<ContactPoint>::iterator pt=manifolds.cPoints.begin(); pt != manifolds.cPoints.end(); pt ++)
    {   
        if (manifolds.cPoints.size() == 0)
            return;
        pt->r_a = pt->pos - pos_a;
        pt->r_b = pt->pos - pos_b;

        // normal to <R> vector
        pt->rn_a = pt->r_a.Cross(collisionNormal);
        pt->rn_b = pt->r_b.Cross(collisionNormal);

        pt->n_impulse = 0;
        pt->t_impulse = 0;
        
        pt->k_normal = m_a + m_b + i_a * pt->rn_a * pt->rn_a + i_b * pt->rn_b * pt->rn_b;
        pt->m_normal = pt->k_normal > 0 ? 1 / pt->k_normal : 0;
        tangent = collisionNormal.Cross(-1);
        pt->rt_a = pt->r_a.Cross(tangent);
        pt->rt_b = pt->r_b.Cross(tangent);
        
        pt->k_tangent = m_a + m_b + i_a * pt->rt_a * pt->rt_a + i_b * pt->rt_b * pt->rt_b;
        pt->m_tangent = pt->k_tangent > 0 ? 1 / pt->k_tangent : 0;

        // for restitution
        pt->v_rel = collisionNormal.Dot(v_b + pt->r_b.Cross(w_b) - (v_a + pt->r_a.Cross(w_a)));
        e = .0;
        pt->bias = 0;

        if (pt->v_rel < -1.5)
            pt->bias = -e * pt->v_rel;

        // for friction
        friction = .0;

        // for position solve
        separation = -penetrationDepth;
    }
}


void Collision::VelocitySolve()
{
    // collision solve using constraint based method.

    // solve tangent constraint for friction.
    v_a = b1->GetVelocity();
    v_b = b2->GetVelocity();
    w_a = b1->GetAngularVelocity();
    w_b = b2->GetAngularVelocity();
    
    for (vector<ContactPoint>::iterator pt=manifolds.cPoints.begin(); pt != manifolds.cPoints.end(); pt++)
    {

        Vector2 dv = v_b + pt->r_b.Cross(w_b) - (v_a + pt->r_a.Cross(w_a));
        SCALAR v_t = dv.Dot(tangent);
        SCALAR lambda = pt->m_tangent * -v_t;
        SCALAR max_friction = friction * pt->n_impulse;
        double new_impulse = Clamp(pt->t_impulse + lambda, -max_friction, max_friction);
        lambda = new_impulse - pt->t_impulse;
        pt->t_impulse = new_impulse;

        Vector2 P = tangent * lambda;
        b1->AddVelocity(-P * m_a);
        b1->AddAngularVelocity(-pt->r_a.Cross(P) * i_a);
        b2->AddVelocity(P * m_b);
        b2->AddAngularVelocity(pt->r_b.Cross(P) * i_b);

        // solve normal constraint for restitution, no penetration.
        SCALAR v_n = collisionNormal.Dot(v_b + pt->r_b.Cross(w_b) - (v_a + pt->r_a.Cross(w_a)));
        lambda = -pt->m_normal * (v_n - pt->bias);
        new_impulse = fmax(pt->n_impulse + lambda, 0);
        lambda = new_impulse - pt->n_impulse;
        pt->n_impulse = new_impulse;
        
        P = collisionNormal * lambda;
        b1->AddVelocity(-P * m_a);
        b1->AddAngularVelocity(-pt->r_a.Cross(P) * i_a);
        b2->AddVelocity(P * m_b);
        b2->AddAngularVelocity(pt->r_b.Cross(P) * i_b);
    }
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

    for (vector<ContactPoint>::iterator pt=manifolds.cPoints.begin(); pt!=manifolds.cPoints.end(); pt++)
    {
        Vector2 r_a = pt->pos - pos_a;
        Vector2 r_b = pt->pos - pos_b;

        // min separation???
        double C = Clamp(bgt * (separation + slop), -max_correction, 0);
        SCALAR rn_a = r_a.Cross(collisionNormal);
        SCALAR rn_b = r_b.Cross(collisionNormal);
        double K = m_a + m_b + i_a * rn_a * rn_a + i_b * rn_b * rn_b;
        SCALAR impulse = K > 0 ? -C / K : 0;
        Vector2 P = collisionNormal * impulse;

        b1->AddPosition(-P * m_a);
        b1->AddRotation(-r_a.Cross(P) * i_a);
        b2->AddPosition(P * m_b);
        b2->AddRotation(r_b.Cross(P) * i_b);
        // b2->AddImpulseAt(P, manifolds.cPoints[0]);
    }
}

void Collision::Push()
{
    // penetration 이 너무 작지 않으면 그 깊이만큼 튕겨내줌.
    if (penetrationDepth > 0.0001)
    {
        Vector2 push = collisionNormal * penetrationDepth * 1;
        b2->AddPosition(push);
    }
}