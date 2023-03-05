# ifndef BODY
# define BODY

# include "../Definition.hpp"
# include "Collider.hpp"

class Body
{
private:
    int id; // 초기화 시 추가
    BodyType type = DYNAMIC; // 초기화 시 추가
    int layer;

    Vector2 position; // = centroid
    SCALAR rotation;
    Vector2 prev_position;
    SCALAR prev_rotation;
    Vector2 velocity = {0.0f, 0.0f};
    SCALAR angularVelocity;
    Vector2 force = {0, 0};
    SCALAR torque = 0;

    // mass data
    SCALAR mass;
    SCALAR inverseMass;
    SCALAR inertia;
    SCALAR inverseInertia;
    double radius;

    Collider* collider;

public:
    Body(POLY_DATA vertices, Vector2 pos, SCALAR rot, int i, SCALAR density, BodyType t);

    // getter
    Vector2 GetPosition();
    SCALAR GetRotation();
    Vector2 GetPrevPosition();
    SCALAR GetPrevRotation();
    Vector2 GetVelocity();
    SCALAR GetAngularVelocity();
    SCALAR GetMass();
    SCALAR GetInertia();
    SCALAR GetInverseMass();
    SCALAR GetInverseInertia();
    BodyType GetType();
    Collider* GetCollider();
    int GetLayer();

    // setter
    void SetPosition(Vector2);
    void SetRotation(SCALAR);
    void SetPrevPosition(Vector2);
    void SetPrevRotation(SCALAR);
    void SetLayer(int);

    void AddForce(Vector2);
    void AddTorque(SCALAR);
    void AddImpulseAt(Vector2 impulse, Vector2 pos);
    void AddRotationalImpulseAt(SCALAR, Vector2);
    void AddJointImpulse(Vector2 v, SCALAR a);
    void AddVelocity(Vector2 v);
    void AddAngularVelocity(SCALAR a);
    void AddPosition(Vector2 pos);
    void AddRotation(SCALAR angle);
    void CalculateVelocity();
    void CalculateAngularVelocity();
    void CalculatePosition();
    void CalculateAngle();
    void ClearForce();
    void ClearTorque();
    void UpdateCentroid();
    
    // for test
    void SetVel(Vector2);
    void SetAngular(SCALAR);
};


# endif