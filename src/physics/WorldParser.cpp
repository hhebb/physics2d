# include "WorldParser.hpp"

// using namespace std;

void WorldParser::Parse(string path)
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

        for (int v; v < vertCount; v ++)
        {
            vector<string> coord = Split(root["BODIES"][i]["VERTICES"][v].asString(), ',');
            Vector2 vec = {stod(coord[0]), stod(coord[1])};
            verts.push_back(vec);
        }
        vector<string> coord = Split(bodies[i]["POSITION"].asString(), ',');
        pos = {stod(coord[0]), stod(coord[1])};
        rot = bodies[i]['ROTATION'].asDouble();

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

    }

}

vector<string> WorldParser::Split(string str, char delim)
{
    vector<string> result;
    stringstream ss (str);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}