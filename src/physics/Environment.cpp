# include <iostream>
# include <random>
# include <pybind11/pybind11.h>
# include <pybind11/stl.h>
# include <pybind11/stl_bind.h>
# include "Environment.hpp"
# include "WorldParser.hpp"

using namespace std;

Environment::Environment(string envName)
{
    // vertices;
    colVelIter = 4;
    colPosIter = 8;
    jointVelIter = 8;
    jointPosIter = 1;

    Parse("/home/hebb/project/physics2d/src/presets/" + envName + ".json");

}

void Environment::Parse(string path)
{
    // get json file
    ifstream ifs(path);
	Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(ifs, root);
    if ( !parsingSuccessful )
    {
        cout  << "Failed to parse configuration\n"
                   << reader.getFormattedErrorMessages();
        return;
    }

    // parse bodies
    testName = root["TEST_NAME"].asString();
    int bodyCount = root["BODIES"].size();
    Json::Value bodies = root["BODIES"];

    for (int i = 0; i < bodyCount; i ++)
    {
        // define
        int t;
        int l;
        POLY_DATA verts;
        Vector2 pos;
        SCALAR rot;

        // parse
        t = bodies[i]["TYPE"].asInt();
        l = bodies[i]["LAYER"].asInt();
        int vertCount = bodies[i]["VERTICES"].size();

        for (int v = 0; v < vertCount; v ++)
        {
            vector<string> coord = Split(root["BODIES"][i]["VERTICES"][v].asString(), ',');
            Vector2 vec = {stod(coord[0]), stod(coord[1])};
            verts.push_back(vec);
        }
        vector<string> coord = Split(bodies[i]["POSITION"].asString(), ',');
        pos = {stod(coord[0]), stod(coord[1])};
        rot = bodies[i]["ROTATION"].asDouble();

        // create body
        Create(verts, pos, rot, 0, (BodyType)t);
        this->bodies[i].SetLayer(l);
    }
    
    // parse joints
    int jointCount = root["JOINTS"].size();
    Json::Value joints = root["JOINTS"];

    for (int i = 0; i < jointCount; i ++)
    {
        // define
        string t;
        int b1;
        int b2;
        Vector2 o1;
        Vector2 o2;

        // parse
        t = joints[i]["TYPE"].asString();
        b1 = joints[i]["BODY_A"].asInt();
        b2 = joints[i]["BODY_B"].asInt();
        vector<string> coord = Split(joints[i]["OFFSET_A"].asString(), ',');
        o1 = {stod(coord[0]), stod(coord[1])};
        coord = Split(joints[i]["OFFSET_B"].asString(), ',');
        o2 = {stod(coord[0]), stod(coord[1])};

        // create joint
        if (t == "PRISMATIC")
        {
            PrismaticJoint* prismaticJoint = new PrismaticJoint(&this->bodies[b1], o1, &this->bodies[b2], o2);
            jointList.push_back(prismaticJoint);
        }
        else if (t == "REVOLUTE")
        {
            RevoluteJoint* revJoint = new RevoluteJoint(&this->bodies[b1], o1, &this->bodies[b2], o2);
            jointList.push_back(revJoint);
        }
        else{}
    }

    // parse action
    int actionCount = root["ACTIONS"].size();
    Json::Value actions = root["ACTIONS"];

    for (int i = 0; i < actionCount; i ++)
    {
        // define
        int body;
        string t;
        SCALAR mag;
        Vector2 p;
        Vector2 d;

        // parse
        body = actions[i]["BODY"].asInt();
        t = actions[i]["TYPE"].asString();
        mag = actions[i]["MAGNITUDE"].asFloat();
        
        vector<string> point = Split(actions[i]["POINT"].asString(), ',');
        vector<string> direc = Split(actions[i]["DIRECTION"].asString(), ',');

        p = {stod(point[0]), stod(point[1])};
        d = {stod(direc[0]), stod(direc[1])};

        // create action instances
        if (t == "DISCRETE")
        {
            Action* action = new Action();
            action->id = body;
            action->point = p;
            action->direction = d;
            action->magnitude = mag;

            actionList.push_back(action);
        }
    }

    // parse states
    statesCount = root["STATES"].size();
    states = root["STATES"];

}

vector<string> Environment::Split(string str, char delim)
{
    vector<string> result;
    stringstream ss (str);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

void Environment::Init(string name)
{
    // 기존 오브젝트 삭제.
    bodies.clear();
    jointList.clear();
    
    // parsing 방식으로 create.
    string path = "/home/hebb/project/physics2d/src/presets/";
    Parse(path + name + ".json");
    SetState(PAUSE);
}

void Environment::Reset()
{
    // 초기 물리 환경 설정으로 초기화.
}

void Environment::SetCommand(Command com)
{
    command = com;
}


void Environment::Create(POLY_DATA ver, Vector2 pos, SCALAR rot, int id, BodyType t)
{
    Body body(ver, pos, rot, id, 1, t);
    this->bodies.push_back(body);
}

void Environment::Debug()
{
    for (int i = 0; i < bodies.size(); i ++)
    {
        Vector2 vel = bodies[i].GetPosition();
    }
}

vector<Body> Environment::GetBodies()
{
    return bodies;
}


bool Environment::IsCollide(Body* body1, Body* body2)
{
    // GJK algorithm.
    Vector2 center1 = body1->GetPosition();
    Vector2 center2 = body2->GetPosition();
    Vector2 direction = center1 - center2;
    Simplex simplex;
    Vector2 a = SupportFunction(body1->GetCollider()->GetVertices(), body2->GetCollider()->GetVertices(), direction);
    simplex.add(a);
    direction = -direction;
    int vertexSum = body1->GetCollider()->GetVertices().size() + body2->GetCollider()->GetVertices().size();
    int count = 0;

    while(vertexSum > count)
    {
        Vector2 b = SupportFunction(body1->GetCollider()->GetVertices(), body2->GetCollider()->GetVertices(), direction);
        simplex.add(b);
        
        if (simplex.GetLastElement().Dot(direction) < 0)
        {
            // origin 을 지나지 않는다는 것이 확실할 때 종료
            return false;
        }
        else
        {
            // origin 지날 때, 아직은 지나지 않을 때
            if (IsContainOrigin(simplex, direction))
            {
                // 이 때 simplex 갖고 있어야 함!
                Collision* collision = new Collision(body1, body2);
                collisionList.push_back(collision);
                collision->FindCollisioninfo(simplex);
                collision->FindManifolds();
                return true;
            }
        }

        count ++;
    }

    return false;
}

Environment::State Environment::GetState()
{
    return state;
}

void Environment::SetState(State s)
{
    state = s;
}

void Environment::SetVelCollisionIter(int value)
{
    colVelIter = value;
}

void Environment::SetPosCollisionIter(int value)
{
    colPosIter = value;
}

void Environment::SetVelJointIter(int value)
{
    jointVelIter = value;
}

void Environment::SetPosJointIter(int value)
{
    jointPosIter = value;
}


void Environment::Step(vector<SCALAR> action)
{
    // physics pipeline.
    // obj 들 update, vertices update

    // generate force
    for (int i = 0; i < bodies.size(); i ++)
    {
        // - force generation
        Vector2 gravity = {.0, -GRAVITY * 1 * bodies[i].GetMass()};
        bodies[i].AddForce(gravity);
    }

    // apply environment command. need initialize.
    {
        // bodies[command.bodyIndex].AddImpulseAt(Vector2{0, 0}, command.applyPoint);
    }

    // integrate velocity
    for (int i = 0; i < bodies.size(); i ++)
    {   
        // - velocity calculation
        bodies[i].CalculateVelocity();
        bodies[i].CalculateAngularVelocity();
        // bodies[i].SetVel(500);

        // - collision detection
        for (int j = i + 1; j < bodies.size(); j ++)
        {
            // filter collision detection.
            if (bodies[i].GetLayer() == bodies[j].GetLayer())
            {
                continue;
            }

            if (IsCollide(&bodies[i], &bodies[j]))
            {
            }
        }
    }

    // action impulse apply
    // vector<SCALAR> tmp_action = proxy.GetAction();
    for (int i = 0; i < actionList.size(); i ++)
    {
        // control test
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<double> dist(/* 평균 = */ 0, /* 표준 편차 = */ 1);
        double rx = (dist(gen)-.5) / 1000.0;
        // double ry = (dist(gen)-.5) / 50.0;

        Vector2 pos = actionList[i]->point + bodies[actionList[i]->id].GetPosition();
        Vector2 impulse = actionList[i]->direction.SimpleRotate(bodies[actionList[i]->id].GetRotation());
        bodies[actionList[i]->id].AddImpulseAt(impulse*rx*actionList[i]->magnitude*action[i], pos);
    }
    

    // init joint
    for (int i = 0; i < jointList.size(); i ++)
    {
        jointList[i]->InitJoint();
    }

    // init collision
    for (int i = 0; i < collisionList.size(); i ++)
    {
        collisionList[i]->InitSolver();
    }

    // joint constraint iterative solve.
    for (int joint_iter = 0; joint_iter < jointVelIter; joint_iter ++)
    {
        for (int i = 0; i < jointList.size(); i ++)
        {
            jointList[i]->VelocitySolve();
        }
    }

    // collision constraint iterative solve.
    for (int solve_iter = 0; solve_iter < colVelIter; solve_iter ++)
    {
        for (int i = 0; i < collisionList.size(); i ++)
        {
            collisionList[i]->VelocitySolve();
        }
    }

    // integrate position.
    for (int i = 0; i < bodies.size(); i ++)
    {
        // - position calculation
        bodies[i].CalculatePosition();
        bodies[i].CalculateAngle();
    }


    // joint solve position
    for (int solve_iter = 0; solve_iter < jointPosIter; solve_iter ++)
    {
        for (int i = 0; i < jointList.size(); i ++)
        {
            jointList[i]->PositionSolve();
        }
    }
    
    // collision solve position
    for (int solve_iter = 0; solve_iter < colPosIter; solve_iter ++)
    {
        for (int i = 0; i < collisionList.size(); i ++)
        {
            collisionList[i]->PositionSolve();
        }
    }


    // update.
    for (int i = 0; i < bodies.size(); i ++)
    {
        Vector2 translateDiff = bodies[i].GetPosition() - bodies[i].GetPrevPosition();
        SCALAR rotateDiff = bodies[i].GetRotation() - bodies[i].GetPrevRotation();
        bodies[i].SetPrevPosition(bodies[i].GetPosition());
        bodies[i].SetPrevRotation(bodies[i].GetRotation());

        // update collider shape, body centroid
        bodies[i].GetCollider()->Update(bodies[i].GetPosition(), translateDiff, rotateDiff);
        bodies[i].UpdateCentroid();

        // - clear forces
        bodies[i].ClearForce();
    }

    // clear collisions
    collisionList.clear();

    // communication with proxy
    vector<Vector2> tmp_state;
    for (int i = 0; i < bodies.size(); i ++)
    {
        tmp_state.push_back(bodies[i].GetPosition());
        tmp_state.push_back(bodies[i].GetVelocity());
    }
}

vector<vector<SCALAR>> Environment::GetEnvRenderVertices()
{
    vertices.clear();
    for (int i = 0; i < bodies.size(); i ++)
    {
        vector<SCALAR> obj;
        for (int j = 0; j < bodies[i].GetCollider()->GetVertices().size(); j ++)
        {
            obj.push_back(bodies[i].GetCollider()->GetVertices()[j].x);
            obj.push_back(bodies[i].GetCollider()->GetVertices()[j].y);
        }

        vertices.push_back(obj);
    }

    return vertices;
}

//

PYBIND11_MODULE(env, m) {
    m.doc() = "environment"; // optional module docstring

    pybind11::enum_<BodyType>(m, "BodyType")
        .value("DYNAMIC", BodyType::DYNAMIC)
        .value("KINEMATIC", BodyType::KINEMATIC)
        .value("STATIC", BodyType::STATIC);

    pybind11::class_<Vector2>(m, "Vector2")
        .def(pybind11::init<SCALAR, SCALAR>())
        .def_readwrite("x", &Vector2::x)
        .def_readwrite("y", &Vector2::y);

    pybind11::class_<Environment>(m, "Environment")
        .def(pybind11::init<string>())
        .def("Step", &Environment::Step)
        .def("GetEnvRenderVertices", &Environment::GetEnvRenderVertices)
        .def("GetBodies", &Environment::GetBodies);

    pybind11::class_<Body>(m, "Body")
        .def(pybind11::init<vector<Vector2>, Vector2, SCALAR, int, SCALAR, BodyType>())
        .def("GetPosition", &Body::GetPosition)
        .def("GetRotation", &Body::GetRotation)
        .def("GetVelocity", &Body::GetVelocity)
        .def("GetAngularVelocity", &Body::GetAngularVelocity);


}