#include "vector2d.h"
#include "rmath.h"

Vector2d::Vector2d(GLfloat xy)
{
    x = xy;
    y = xy;
}
Vector2d::Vector2d(GLfloat x_value, GLfloat y_value)
{
    x=x_value;
    y=y_value;
}

GLfloat Vector2d::getX() const
{
    return x;
}

GLfloat Vector2d::getY() const
{
    return y;
}

void Vector2d::setX(GLfloat value)
{
    x = value;
}

void Vector2d::setY(GLfloat value)
{
    y = value;
}

Vector2d& Vector2d::operator = (const Vector2d& v)
{
    x = v.x;
    y = v.y;

    return *this;
}

Vector2d Vector2d::operator + (const Vector2d& v) const
{
    return Vector2d {x+v.x, y+v.y};
}

Vector2d Vector2d::operator * (const GLfloat& v) const
{
    return Vector2d {x*v, y*v};
}

Vector2d Vector2d::operator - (const Vector2d& v) const
{
    GLfloat a = (x - v.x);
    GLfloat b = (y - v.y);

    return Vector2d {a, b};
}

Vector3d Vector2d::operator ^ (const Vector2d& v) const
{

    return Vector3d( 0, 0, (v.x * y) - (x * v.y));
}

Vector3d Vector2d::barycentricCoordinates(const Vector2d& p1, const Vector2d& p2, const Vector2d& p3)
{
    Vector3d baryc; // til retur. Husk

    Vector2d p12 = p2-p1;
    Vector2d p13 = p3-p1;
    Vector3d n = p12^p13;
    float areal_123 = n.length(); // dobbelt areal

    // u
    Vector2d p = (p2 - *this);
    Vector2d q = (p3 - *this);
    n = p^q;
    baryc.setX(n.getZ()/areal_123);

    // v
    p = p3 - *this;
    q = p1 - *this;
    n = p^q;
    baryc.setY(n.getZ()/areal_123);

    // w
    p = p1 - *this;
    q = p2 - *this;
    n = p^q;
    baryc.setZ(n.getZ()/areal_123);

    return baryc;
}

Vector2d Vector2d::baryNeighbour(const Vector2d& p1, const Vector2d& p2, const Vector2d& p3)
{
    Vector2d neighbouringPoing; // to retur

    // make point on line
    neighbouringPoing = (p2*0.5f)+(p3*0.5f);
    // push point over the line
    float t = 1.5f;
    neighbouringPoing = (p1*(1-t))+(neighbouringPoing*t);


    return neighbouringPoing;
}

void Vector2d::print()
{
        qDebug() << x << " " << y;
}
