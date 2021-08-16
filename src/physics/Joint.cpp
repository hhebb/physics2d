# include "Joint.hpp"

Joint::Joint(Body* a, Vector2 offset_a, Body* b, Vector2 offset_b)
{
    bodyA = a;
    bodyB = b;
    offsetA = offset_a;
    offsetB = offset_b;   
}

void Joint::InitJoint()
{

}

void Joint::VelocitySolve()
{

}

void Joint::PositionSolve()
{
    
}

void Joint::ApplyJointImpulse(){}