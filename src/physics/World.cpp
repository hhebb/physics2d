# include "World.hpp"
# include "WorldParser.hpp"
# include <iostream>
using namespace std;

World::World()
{
    // vertices;
    // Init();
    // Reset();
    colVelIter = 4;
    colPosIter = 8;
    jointVelIter = 8;
    jointPosIter = 1;
}

void World::Parse(string path)
{
    // "/home/hebb/project/rlstudio/test/test.json"
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
        RevoluteJoint* revJoint = new RevoluteJoint(&this->bodies[b1], o1, &this->bodies[b2], o2);
        jointList.push_back(revJoint);

    }
}

vector<string> World::Split(string str, char delim)
{
    vector<string> result;
    stringstream ss (str);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

void World::Init()
{
    // parsing 방식으로 create.
    Parse("/home/hebb/project/physics2d/test/peTest/presets/test.json");

    
    // body 들 생성, 물성치 설정. 모든 물리 환경 설정.
    // POLY_DATA vert; // vertices
    // Vector2 pos; // center
    // SCALAR rot;
    // SCALAR mass;
    // float radius;

    // // 천장
    // pos = {0.0, 0.65};
    // vert = {Vector2{0.0, 0.0}, Vector2{2, .0}, Vector2{2, .1}, Vector2{0.0, .1}}; // x, y order
    // rot = 0;
    // Create(vert, pos, rot, 0, STATIC);
    // // bodies[0].SetVel(Vector2{0, 5.9});
    // // bodies[0].SetAngular(500);
    // bodies[0].SetLayer(0);

    // // 바닥
    // pos = {-0.1, 0.0};
    // vert = {Vector2{0.0, 0.0}, Vector2{.5, .0}, Vector2{.5, .1}, Vector2{0.0, .1}}; // x, y order
    // rot = 45 * DEGREE_TO_RADIAN;
    // Create(vert, pos, rot, 0, DYNAMIC);
    // bodies[1].SetLayer(0);

    // // 바닥
    // pos = {-0.1, 0.0};
    // vert = {Vector2{0.0, 0.0}, Vector2{.5, .0}, Vector2{.5, .1}, Vector2{0.0, .1}}; // x, y order
    // rot = -45 * DEGREE_TO_RADIAN;
    // Create(vert, pos, rot, 0, DYNAMIC);
    // bodies[2].SetLayer(0);

    // // 바닥
    // pos = {-0.1, 0.0};
    // vert = {Vector2{0.0, 0.0}, Vector2{.5, .0}, Vector2{.5, .1}, Vector2{0.0, .1}}; // x, y order
    // rot = 0 * DEGREE_TO_RADIAN;
    // Create(vert, pos, rot, 0, DYNAMIC);
    // bodies[3].SetLayer(0);

    // // 예시 body
    // pos = {0.2, -0.0};
    // // vert = {Vector2{0.0, 0.0}, Vector2{.2, .0}, Vector2{.2, .2}, Vector2{0.0, .2}}; // x, y order
    // // vert = {Vector2{-0.2, 0.0}, Vector2{.4, .0}, Vector2{.2, .2}, Vector2{0.0, .2}}; // x, y order
    // vert = {Vector2{0.0, 0.0}, Vector2{.2, 0.0}, Vector2{.13, .2}}; // x, y order
    // rot = 60 * DEGREE_TO_RADIAN;
    // // Create(vert, pos, rot, 1, DYNAMIC);
    // // bodies[2].SetVel(Vector2{.0, -1.0});
    // // bodies[2].SetAngular(10 * DEGREE_TO_RADIAN);
    // // bodies[2].SetLayer(2);

    // // 예시 body
    // pos = {-0.2, 0.5};
    // vert = {Vector2{0.0, 0.0}, Vector2{.4, .0}, Vector2{.4, .4}, Vector2{0.0, .4}}; // x, y order
    // // vert = {Vector2{-0.2, 0.0}, Vector2{.4, .0}, Vector2{.2, .2}, Vector2{0.0, .2}}; // x, y order
    // // vert = {Vector2{0.0, 0.0}, Vector2{.2, 0.0}, Vector2{.13, .2}}; // x, y order
    // rot = 10 * DEGREE_TO_RADIAN;
    // // Create(vert, pos, rot, 2, DYNAMIC);
    // // bodies[3].SetVel(Vector2{.0, -.0});
    // // bodies[3].SetAngular(-100 * DEGREE_TO_RADIAN);
    // // bodies[3].SetLayer(2);

    // // 예시 body
    // pos = {0.5, 0.5};
    // // vert = {Vector2{0.0, 0.0}, Vector2{.4, .0}, Vector2{.4, .4}, Vector2{0.0, .4}}; // x, y order
    // vert = {Vector2{-0.2, 0.0}, Vector2{.2, .0}, Vector2{.2, .2}, Vector2{0.0, .2}}; // x, y order
    // // vert = {Vector2{0.0, 0.0}, Vector2{.2, 0.0}, Vector2{.13, .2}}; // x, y order
    // rot = -0 * DEGREE_TO_RADIAN;
    // // Create(vert, pos, rot, 2, DYNAMIC);
    // // bodies[3].SetVel(Vector2{.5, .0});
    // // bodies[2].SetAngular(50);
    // // bodies[4].SetLayer(4);

    // RevoluteJoint* revJoint = new RevoluteJoint(&bodies[0], Vector2{0.0, -0.0}, &bodies[1], Vector2{-0.25, 0.0});
    // RevoluteJoint* revJoint2 = new RevoluteJoint(&bodies[1], Vector2{0.25, -0.0}, &bodies[2], Vector2{-0.25, 0.0});
    // RevoluteJoint* revJoint3 = new RevoluteJoint(&bodies[2], Vector2{0.25, -0.0}, &bodies[3], Vector2{-0.25, 0.0});
    // // DistanceJoint* distJoint = new DistanceJoint(&bodies[0], Vector2{-0.5, .5}, &bodies[2], Vector2{-0.0, .0});
    // jointList.push_back(revJoint);
    // jointList.push_back(revJoint2);
    // jointList.push_back(revJoint3);
}

void World::Reset()
{
    // 초기 물리 환경 설정으로 초기화.
}

void World::Step()
{
    // physics pipeline.
    // obj 들 update, vertices update

    // generate force
    for (int i = 0; i < bodies.size(); i ++)
    {
        // - force generation
        Vector2 gravity = {.0, -GRAVITY * 1 * bodies[i].GetMass()};
        SCALAR torque = 1;
        bodies[i].AddForce(gravity);
        // bodies[i].AddTorque(torque);
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
                cout << "> collide!" << endl;
            }
        }
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
        cout << "> collision init check" << i << endl;
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
            // cout << "> collision velocity solve check" << i << endl;
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
            // cout << "> collision position solve check" << i << endl;
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
}

void World::SetCommand(Command com)
{
    command = com;
}

POLY_LIST World::GetVertices()
{
    return this->vertices;
}

void World::Create(POLY_DATA ver, Vector2 pos, SCALAR rot, int id, BodyType t)
{
    Body body(ver, pos, rot, id, 1, t);
    this->bodies.push_back(body);
    // this->vertices.push_back(ver.data());
}

void World::Debug()
{
    for (int i = 0; i < bodies.size(); i ++)
    {
        // cout << "Body " << i + 1 << " : ";
        Vector2 vel = bodies[i].GetPosition();
        // cout << vel.x << ", " << vel.y << endl;
    }
}

vector<Body> World::GetBodies()
{
    return bodies;
}

void World::run()
{
    // 이 메서드에서 스레드 work 를 실행한다.
    // 모든 물리 연산은 여기서 수행되고 결과를 emit 한다.
    // GL window 가 vsync 가 될 때까지 대기를 하므로, 렌더링을 켜면 연산이 느려진다.
    int count = 0;
    while(count < 1)
    {
        // main physics.
        while(!ready) {}
        ready = false;
        count ++;
        Step();

        // emit for rendering.
        QVariant var;
        vertices.clear();

        for (int i = 0; i < bodies.size(); i ++)
        {
            // var.setValue(bodies[i].GetCollider()->GetVertices());
            vertices.push_back(bodies[i].GetCollider()->GetVertices());
        }
        var.setValue(vertices);
        emit physicsUpdate(var);
    }
}

bool World::IsCollide(Body* body1, Body* body2)
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
        // cout << "> colliding check" << endl;
        Vector2 b = SupportFunction(body1->GetCollider()->GetVertices(), body2->GetCollider()->GetVertices(), direction);
        simplex.add(b);
        
        // cout << "count: " << count << endl;
        
        if (simplex.GetLastElement().Dot(direction) < 0)
        {
            // origin 을 지나지 않는다는 것이 확실할 때 종료
            // cout << "not collide" << endl;
            // PrintVector("last: ", simplex.GetLastElement());
            // PrintVector("dir: ", direction);
            return false;
        }
        else
        {
            // PrintVector("contain, last: ", simplex.GetLastElement());
            // PrintVector("contain, dir: ", direction);
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
            // PrintVector("changed dir", direction);

        }

        count ++;
    }

    return false;
}
