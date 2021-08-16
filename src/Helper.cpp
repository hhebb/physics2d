# include "Helper.hpp"
# include <limits>

Vector2 GetFarthestVertex(POLY_DATA verts, Vector2 d)
{
    double max = -numeric_limits<double>::max();
    int maxIdx = -1;
    for (int i = 0; i < verts.size(); i ++)
    {
        double dist = verts[i].Dot(d);
        if (dist > max)
        {
            max = dist;
            maxIdx = i;
        }
    }

    return verts[maxIdx];
}

Vector2 SupportFunction(POLY_DATA verts1, POLY_DATA verts2, Vector2 d)
{
    Vector2 a = GetFarthestVertex(verts1, d);
    Vector2 b = GetFarthestVertex(verts2, -d);
    
    return a - b;
}


Vector2 TripleProduct(Vector2 a, Vector2 b, Vector2 c)
{
    return b * c.Dot(a) - a * c.Dot(b);
}

bool IsContainOrigin(Simplex& simplex, Vector2& d)
{
    Vector2 a = simplex.GetLastElement();
    
    if (simplex.elements.size() == 3)
    {
        Vector2 b = simplex.GetB();
        Vector2 c = simplex.GetC();

        Vector2 ab = b - a;
        Vector2 ac = c - a;

        Vector2 abPerp = TripleProduct(ac, ab, ab);
        Vector2 acPerp = TripleProduct(ab, ac, ac);

        if (abPerp.Dot(-a) > 0)
        {
            simplex.Remove(2);
            d.Set(abPerp);
            // PrintVector("ab perp", abPerp);
        }
        else
        {
            if (acPerp.Dot(-a) > 0)
            {
                simplex.Remove(1);
                d.Set(acPerp);
                // PrintVector("ac perp", acPerp);

            }
            else
            {
                // already contain origin. dont need to change direction
                // cout << "> contain origin" << endl;
                return true;
            }
        }

    }
    else if (simplex.elements.size() == 2)
    {
        Vector2 b = simplex.GetB();
        Vector2 ab = b - a;
        Vector2 abPerp = TripleProduct(ab, -a, ab);
        d.Set(abPerp);
        // PrintVector("ab perp 2", abPerp);

    }
    else
    {
        // cout << "only one size!!!!!" << endl;
    }

    return false;
}

Vector2 Edge::GetVector()
{
    return b - a;
}

Edge FindClosetEdge(Simplex simplex)
{
    Edge closest;
    closest.distance = numeric_limits<double>::max();
    for (int i = 0; i < simplex.elements.size(); i ++)
    {
        int j = i + 1 == simplex.elements.size() ? 0 : i + 1;
        Vector2 a = simplex.elements[i];
        Vector2 b = simplex.elements[j];
        Vector2 e = b - a;
        Vector2 normal = TripleProduct(e, a, e);
        normal = normal.Normalise();
        double dist = normal.Dot(a);
        if (dist < closest.distance)
        {
            closest.distance = dist;
            closest.index = j;
            closest.normal = normal;
        }
    }

    return closest;
}

double Clamp(double val, double min, double max)
{
    if (val < min)
    {
        return min;
    }
    else if (val > max)
    {
        return max;
    }
    else return val;
}

void PrintVector(string desc, Vector2 vec)
{
    cout << "> "  << desc  << ": " << vec.x << ", " << vec.y << endl;
}

void PrintScalar(string desc, SCALAR s)
{
    cout << "> "  << desc  << ": " << s << endl;
}