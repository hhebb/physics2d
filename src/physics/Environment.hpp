# ifndef ENVIRONMENT
# define ENVIRONMENT

# include <QThread>
# include <QVariant>
# include <jsoncpp/json/json.h>
# include <fstream>
# include <iostream>
# include <sstream>

// # include "proxy.hpp"
# include "Body.hpp"
# include "Collider.hpp"
# include "Collision.hpp"
# include "../Helper.hpp"
# include "Joint.hpp"
# include "RevoluteJoint.hpp"
# include "DistanceJoint.hpp"
# include "PrismaticJoint.hpp"
# include "envs/EnvHeaders.hpp"

class Environment: public QThread
{
    Q_OBJECT

signals:
    void physicsUpdate(QVariant);
private:
    
    // custom parse
    string testName;
    vector<POLY_DATA> polys;
    vector<Vector2> positions;
    vector<SCALAR> rotations;
    vector<BodyType> bTypes;
    vector<int> layers;
    int statesCount;
    Json::Value states;
    BaseEnv* env;

    // Proxy proxy;
    vector<Body> bodies;
    Command command;
    POLY_LIST vertices;
    vector<Collision*> collisionList; // collision manager 만들어서 관리하도록 수정해야함.
    vector<Joint*> jointList; // manager 만들어서 관리하도록 수정해야함.
    vector<Action*> actionList;
    vector<SCALAR> stateList;
    int colVelIter;
    int colPosIter;
    int jointVelIter;
    int jointPosIter;

public:
    enum State {PLAY, PAUSE, SINGLE_STEP};
    State state;
    bool ready;
    Environment();
    void Parse(string path);
    void FilterState();
    vector<string> Split(string str, char delim);
    State GetState();
    void Init(string name);
    void Reset();
    void SetCommand(Command com);
    POLY_LIST GetVertices(); // for rendering
    // id 리턴하도록 바꾸기. create.
    void Create(POLY_DATA ver, Vector2 pos, SCALAR rot, int id, BodyType t);
    void Debug();
    vector<Body> GetBodies();
    bool IsCollide(Body*, Body*);
    
    // command 처리.
    void SetState(State);
    void SetVelCollisionIter(int value);
    void SetPosCollisionIter(int value);
    void SetVelJointIter(int value);
    void SetPosJointIter(int value);

    // step
    void Step(vector<SCALAR> action);
    vector<SCALAR> GetEnvState();
    SCALAR GetEnvReward();
    bool GetEnvIsDone();
    vector<SCALAR> GetEnvInfo();
    
    
};

# endif