# ifndef HELPER
# define HELPER

# include "Definition.hpp"
# include <string>

Vector2 GetFarthestVertex(POLY_DATA, Vector2);
Vector2 SupportFunction(POLY_DATA, POLY_DATA, Vector2);
Vector2 TripleProduct(Vector2, Vector2, Vector2);
bool IsContainOrigin(Simplex&, Vector2&);
void PrintVector(string, Vector2);
void PrintScalar(string, SCALAR);

struct Edge
{
    Vector2 a;
    Vector2 b;
    Vector2 farthest;
    Vector2 e; // ?? 걍 getter 로 가져오기
    Vector2 normal;
    double distance;
    int index;
    Vector2 GetVector();
};

Edge FindClosetEdge(Simplex);

double Clamp(double, double, double);


# endif