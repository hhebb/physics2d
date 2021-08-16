# ifndef WORLD_PARSER
# define WORLD_PARSER

# include <jsoncpp/json/json.h>
# include <fstream>
# include <iostream>
# include <sstream>
# include "../Definition.hpp"

using namespace std;

class WorldParser
{
public:
    string testName;
    vector<POLY_DATA> polys;
    vector<Vector2> positions;
    vector<SCALAR> rotations;
    vector<BodyType> bTypes;
    vector<int> layers;

    void Parse(string path);
    vector<string> Split(string str, char delim);
};


# endif