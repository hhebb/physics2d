# include "Body.hpp"
# include <iostream>
using namespace std;

Body::Body(POLY_DATA vertices, Vector2 pos, SCALAR rot, int i, SCALAR density, BodyType t)
{
    // set collider
    // calc collider's centroid, move vertices.
    id = i;
    position = pos; // centroid 로 대체된다.
    rotation = rot;
    prev_position = position;
    prev_rotation = rotation;
    collider = new Collider(vertices, position, rotation, radius);
    type = t;
    // PrintScalar("type", t);
    
    // get mass data from created collider.
    mass = type == DYNAMIC ? collider->CalculateMass(density) : 0;
    inverseMass = mass == 0 ? 0 : 1 / mass;
    inertia = type == DYNAMIC ? collider->CalculateInertia(position, density) : 0;
    inverseInertia = inertia == 0 ? 0 : 1 / inertia;
    // PrintScalar("mass", mass);
    // PrintScalar("i_mass", inverseMass);
    // PrintScalar("inertia", inertia);
    // PrintScalar("i_intertia", inverseInertia);
}

#pragma region GETTER
Vector2 Body::GetPosition()
{
    return position;
}

SCALAR Body::GetRotation()
{
    return rotation;
}

Vector2 Body::GetPrevPosition()
{
    return prev_position;
}

SCALAR Body::GetPrevRotation()
{
    return prev_rotation;
}

Vector2 Body::GetVelocity()
{
    return velocity;
}

SCALAR Body::GetAngularVelocity()
{
    return angularVelocity;
}

SCALAR Body::GetMass()
{
    return mass;
}

SCALAR Body::GetInertia()
{
    return inertia;
}

SCALAR Body::GetInverseMass()
{
    return inverseMass;
}

SCALAR Body::GetInverseInertia()
{
    return inverseInertia;
}

BodyType Body::GetType()
{
    return type;
}

Collider* Body::GetCollider()
{
    return collider;
}

int Body::GetLayer()
{
    return layer;
}

#pragma endregion

void Body::SetPosition(Vector2 pos)
{
    position = pos;
    collider->SetPosition(position);
}

void Body::SetRotation(SCALAR rot)
{
    rotation = rot;
    collider->SetRotation(rotation);
}

void Body::SetPrevPosition(Vector2 pos)
{
    prev_position = pos;
}

void Body::SetPrevRotation(SCALAR rot)
{
    prev_rotation = rot;
}

void Body::SetLayer(int l)
{
    layer = l;
}

#pragma region CALCULATE
void Body::AddForce(Vector2 f)
{
    force += f;
}

void Body::AddTorque(SCALAR t)
{
    torque += t;
}

void Body::AddImpulseAt(Vector2 impulse, Vector2 pos)
{
    // dynamics for impulse at arbitrary point.
    // for collision solve.
    SCALAR rotation_impulse = (pos - position).Cross(impulse);

    velocity += impulse * inverseMass;
    angularVelocity += rotation_impulse * inverseInertia;
}

// void Body::AddRotationalImpulseAt(SCALAR rotImpulse, Vector2 pos)
// {
//     // dynamics for rotaitonal impulse at arbitrary point.
//     // for collision solve.
//     Vector2 localPos = position + pos;
//     Vector2 r = Vector2(localPos - position);
//     Vector2 J = r.Cross(-rotImpulse); // perp vector to r_vector.
//     double M = r.Cross(J);


//     // SCALAR rotation_impulse = rotImpulse;

//     velocity += J * inverseMass;
//     angularVelocity += M * inverseInertia;
// }

void Body::AddJointImpulse(Vector2 v, SCALAR a)
{
    velocity += v;
    angularVelocity += a;
}

void Body::AddVelocity(Vector2 v)
{
    velocity += v;
}

void Body::AddAngularVelocity(SCALAR a)
{
    angularVelocity += a;
}

void Body::AddPosition(Vector2 pos)
{
    position += pos;
}

void Body::AddRotation(SCALAR angle)
{
    rotation += angle;
}

void Body::CalculateVelocity()
{
    velocity += force * inverseMass * DELTA_TIME;
}

void Body::CalculateAngularVelocity()
{
    angularVelocity += torque * inverseInertia * DELTA_TIME;
}

void Body::CalculatePosition()
{
    position += velocity * DELTA_TIME;
}

void Body::CalculateAngle()
{
    rotation += angularVelocity * DELTA_TIME;
}

void Body::ClearForce()
{
    force = {0, 0};
}

void Body::ClearTorque()
{
    torque = 0;
}

void Body::UpdateCentroid()
{
    position = collider->GetCentroid();
}

#pragma endregion

// test
void Body::SetVel(Vector2 vel)
{
    velocity = vel;
}

void Body::SetAngular(SCALAR w)
{
    angularVelocity = w;
}