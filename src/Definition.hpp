# ifndef DEFINITION
# define DEFINITION

# include <vector>
# include <queue>
# include <math.h>
# include <QMetaType>
# include <iostream>

using namespace std;

# define SCALAR double
# define DELTA_TIME 0.016
# define GRAVITY 9.81
# define POLY_DATA vector<Vector2>
# define VERTEX_LIST vector<Vector2>
# define POLY_LIST vector<POLY_DATA>
# define PI 3.141592
# define DEGREE_TO_RADIAN 0.0174533 //PI / 180.0
# define TOLERANCE 0.00001

using namespace std;

enum BodyType {DYNAMIC, KINEMATIC, STATIC};

struct Vector2
{
    double x;
    double y;

    void Set(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    void Set(Vector2 vec)
    {
        this->x = vec.x;
        this->y = vec.y;
    }

    SCALAR Dot(Vector2 vec)
    {
        return SCALAR{x * vec.x + y * vec.y};
    }

    Vector2 Cross(double z)
    {
        // get perpendicular vector.
        // display direction is positive z-axis.
        return Vector2{-1.0 * y * z, x * z};
    }

    SCALAR Cross(Vector2 vec)
    {
        // get magnitude of cross product.
        // same as area of trapezoid.
        return x * vec.y - y * vec.x;
    }

    SCALAR GetSqrLength()
    {
        return x * x + y * y;
    }

    SCALAR GetLength()
    {
        return sqrt(x * x + y * y);
    }

    Vector2 Normalise()
    {
        double factor = sqrt(x * x + y * y);
        return Vector2{x / factor, y / factor};
    }
    
    Vector2 SimpleTranslate(Vector2 delta)
    {
        return Vector2{x + delta.x, y + delta.y};
    }

    Vector2 SimpleRotate(SCALAR angle, bool inverse=false)
    {
        double c = cos(angle);
        double s = sin(angle);

        if (inverse)
            return Vector2{c * x + s * y, s * x - c * y};
        else
            return Vector2{c * x - s * y, s * x + c * y};
    }

    // operator
    Vector2 operator-()
    {
        return Vector2{-x, -y};
    }

    Vector2 operator+(Vector2 vec)
    {
        return Vector2{x + vec.x, y + vec.y};
    }

    Vector2 operator-(Vector2 vec)
    {
        return Vector2{x - vec.x, y - vec.y};
    }

    Vector2 operator*(SCALAR scale)
    {
        return Vector2{x * scale, y * scale};
    }

    Vector2 operator/(SCALAR div)
    {
        return Vector2{x / div, y / div};
    }

    Vector2& operator+=(const Vector2& vec)
    {
        (*this) = (*this) + vec;
        return *this;
    }

    Vector2& operator-=(const Vector2& vec)
    {
        (*this) = (*this) - vec;
        return *this;
    }

    Vector2& operator*=(const SCALAR& scale)
    {
        (*this) = (*this) * scale;
        return *this;
    }

    Vector2& operator/=(const SCALAR& scale)
    {
        (*this) = (*this) / scale;
        return *this;
    }
};

struct Vector3
{
    double x, y, z;
    void Set(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

struct Matrix2x2
{
    double m11 = 0, m12 = 0, m21 = 0, m22 = 0;

    void Set(double a, double b, double c, double d)
    {
        m11 = a;
        m12 = b;
        m21 = c;
        m22 = d;
    }

    // solve Ax = b matrix equation. return x vector.
    Vector2 Solve(Vector2 vec)
    {
        double det = m11 * m22 - m12 * m21;
        if (det == 0)
        {
            // cout << "det = 0!" << endl;
            return Vector2{0, 0};
        }

        Vector2 x = {m22 * vec.x - m12 * vec.y, -m21 * vec.x + m11 * vec.y};
        x *= (1 / det);

        return x;
    }
};

struct Matrix3x3
{
    double m11 = 0, m12 = 0, m13 = 0, m21 = 0, m22 = 0, m23 = 0, m31 = 0, m32 = 0, m33 = 0;

    void Set(vector<double> r1, vector<double> r2, vector<double> r3)
    {
        m11, m12, m13 = r1[0], r1[2], r1[3];
        m21, m22, m23 = r2[0], r2[2], r2[3];
        m31, m32, m33 = r3[0], r3[2], r3[3];
    }

    // solve Ax = b matrix equation. return x vector.
    Vector3 Solve(Vector3 vec)
    {
        double det = m11 * (m22 * m33 - m23 * m32) - m12 * (m21 * m33 - m23 * m31) + m13 * (m21 * m32 - m22 * m31);
        if (det == 0)
        {
            // cout << "det = 0!" << endl;
            return Vector3{0, 0, 0};
        }

        double x = (m22 * m33 - m23 * m32) * vec.x - (m12 * m33 - m13 * m32) * vec.y + (m12 * m23 - m13 * m22) * vec.z;
        double y = -(m21 * m33 - m23 * m31) * vec.x + (m11 * m33 - m13 * m31) * vec.y - (m11 * m23 - m13 * m21) * vec.z;
        double z = (m21 * m32 - m22 * m31) * vec.x - (m11 * m32 - m12 * m31) * vec.y + (m11 * m22 - m13 * m31) * vec.z;
        x = x /det;
        y = y /det;
        z = z /det;

        Vector3 v{x, y, z};
        return v;
    }
};

// homogeneous transform 3x3 matrix for 2D.
struct HomogeneousMatrix3x3 //HomogeneousMatrix
{
    double m11, m12, m13, m21, m22, m23, m31, m32, m33;
    Vector2 center;

    void SetIdentity()
    {
        m11 = 1, m22 = 1, m33 = 1;
        m12 = 0, m13 = 0, m21 = 0, m23 = 0, m31 = 0, m32 = 0;
    }

    void Translate(Vector2 translate)
    {
        m11 = m11 + translate.x * m31;
        m12 = m12 + translate.x * m32;
        m13 = m13 + translate.x * m33;
        m21 = m21 + translate.y * m31;
        m22 = m22 + translate.y * m32;
        m23 = m23 + translate.y * m33;
    }

    void Rotate(SCALAR angle, Vector2 center)
    {
        // radian based
        SCALAR radAngle = angle;
        double c = cos(radAngle);
        double s = sin(radAngle);

        double e1 = center.x * (1.0 - c) + center.y * s;
        double e2 = center.y * (1.0 - c) - center.x * s;

        double tmp_m11, tmp_m12, tmp_m13;

        tmp_m11 = c * m11 - s * m21 + e1 * m31;
        tmp_m12 = c * m12 - s * m22 + e1 * m32;
        tmp_m13 = c * m13 - s * m23 + e1 * m33;
        m21 = s * m11 + c * m21 + e2 * m31;
        m22 = s * m12 + c * m22 + e2 * m32;
        m23 = s * m13 + c * m23 + e2 * m33;

        m11 = tmp_m11;
        m12 = tmp_m12;
        m13 = tmp_m13;

    }

    void Scale(Vector2 scale)
    {
        m11 *= scale.x;
        m12 *= scale.y;
        m21 *= scale.x;
        m22 *= scale.y;
        m31 *= scale.x;
        m32 *= scale.y;
    }

    void Scale(SCALAR scale)
    {
        m11 *= scale;
        m12 *= scale;
        m21 *= scale;
        m22 *= scale;
        m31 *= scale;
        m32 *= scale;
    }

    // return transformed all vertices of one body's
    POLY_DATA Multiply(POLY_DATA vertices)
    {
        POLY_DATA result;
        Vector2 tmp;
        for (int i = 0 ; i < vertices.size(); i ++)
        {
            tmp.x = m11 * vertices[i].x + m12 * vertices[i].y + m13;
            tmp.y = m21 * vertices[i].x + m22 * vertices[i].y + m23;
            result.push_back(tmp);
        }

        return result;
    }

    Vector2 Multiply(Vector2 vec)
    {
        Vector2 tmp;
        tmp.x = m11 * vec.x + m12 * vec.y + m13;
        tmp.y = m21 * vec.x + m22 * vec.y + m23;
        return tmp;
    }
};

struct Simplex
{
    // float a, b, c;
    vector<Vector2> elements;

    void add(Vector2 v)
    {
        elements.push_back(v);
    }

    Vector2 GetLastElement()
    {
        return elements[elements.size() - 1];
    }

    Vector2 GetB()
    {
        return elements[elements.size() - 2];
    }

    Vector2 GetC()
    {
        return elements[0];
    }

    void Remove(int index)
    {
        elements.erase(elements.begin() + index);
    }

    void Insert(Vector2 v, int index)
    {
        elements.insert(elements.begin() + index, v);
    }

};

struct ClippedPoints
{
    VERTEX_LIST cPoints;
};

struct Command
{
    int bodyIndex;
    int commandType; // motor, push impulse.
    Vector2 applyPoint;

};

Q_DECLARE_METATYPE(Vector2);
Q_DECLARE_METATYPE(Vector3);
Q_DECLARE_METATYPE(Matrix2x2);
Q_DECLARE_METATYPE(HomogeneousMatrix3x3);

# endif