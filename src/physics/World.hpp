# ifndef WORLD
# define WORLD

# include <QThread>
# include <QVariant>
# include "Body.hpp"
# include "Collider.hpp"
# include "Collision.hpp"
# include "../Helper.hpp"
# include "Joint.hpp"
# include "RevoluteJoint.hpp"
# include "DistanceJoint.hpp"

class World: public QThread
{
    Q_OBJECT

signals:
    void physicsUpdate(QVariant);
private:
    vector<Body> bodies;
    Command command;
    POLY_LIST vertices;
    vector<Collision*> collisionList; // collision manager 만들어서 관리하도록 수정해야함.
    vector<Joint*> jointList; // manager 만들어서 관리하도록 수정해야함.
    void run();
    int colVelIter;
    int colPosIter;
    int jointVelIter;
    int jointPosIter;

public:
    bool ready;
    World();
    void Init();
    void Reset();
    void Step();
    void SetCommand(Command com);
    POLY_LIST GetVertices(); // for rendering
    // id 리턴하도록 바꾸기. create.
    void Create(POLY_DATA ver, Vector2 pos, SCALAR rot, int id, BodyType t);
    void Debug();
    vector<Body> GetBodies();
    bool IsCollide(Body*, Body*);
    
    
};

# endif