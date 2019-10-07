#include "vector3d.h"
#include "vector4d.h"
#include "vector2d.h"
#include <QDebug>

Vector3d::Vector3d(GLfloat values)
{
    x=values;
    y=values;
    z=values;
}

Vector3d::Vector3d(GLfloat x_value, GLfloat y_value, GLfloat z_value)
{
    x=x_value;
    y=y_value;
    z=z_value;
}

Vector3d::Vector3d(Vector4d vec)
{
    x=vec.x;
    y=vec.y;
    z=vec.z;
}

// operator overloads
Vector3d Vector3d::operator*(Vector3d value)
{
    GLfloat u = x * value.x;
    GLfloat v = y * value.y;
    GLfloat w = z * value.z;

    return Vector3d {u, v, w};
}

Vector3d Vector3d::operator*(GLfloat value)
{
    GLfloat u = x * value;
    GLfloat v = y * value;
    GLfloat w = z * value;

    return Vector3d {u, v, w};
}

Vector3d Vector3d::operator/(GLfloat value)
{
    GLfloat u = x / value;
    GLfloat v = y / value;
    GLfloat w = z / value;

    return Vector3d {u, v, w};
}

Vector3d Vector3d::operator+(GLfloat value)
{
    GLfloat u = x + value;
    GLfloat v = y + value;
    GLfloat w = z + value;

    return Vector3d {u, v, w};
}

Vector3d Vector3d::operator-(GLfloat value)
{
    GLfloat u = x - value;
    GLfloat v = y - value;
    GLfloat w = z - value;

    return Vector3d {u, v, w};
}

Vector3d Vector3d::operator-() const
{

    return Vector3d {-x, -y, -z};
}

const Vector3d& Vector3d::operator=(const Vector3d& v)
{
    x = v.x;
    y = v.y;
    z = v.z;

    return *this;
}

const Vector3d& Vector3d::operator+=(const Vector3d& v)
{
    x += v.x;
    y += v.y;
    z += v.z;

    return *this;
}

Vector3d Vector3d::operator + (const Vector3d& v) const
{
    GLfloat a = (x + v.x);
    GLfloat b = (y + v.y);
    GLfloat c = (z + v.z);

    return Vector3d {a, b, c};
}

Vector3d Vector3d::operator - (const Vector3d& v) const
{
    GLfloat a = (x - v.x);
    GLfloat b = (y - v.y);
    GLfloat c = (z - v.z);

    return Vector3d {a, b, c};
}

float Vector3d::operator * (const Vector3d& v) const
{
    GLfloat a = (x * v.x)+(y * v.y)+(z * v.z);

    return a;
}

Vector3d Vector3d::operator ^ (const Vector3d& v) const
{
    return Vector3d( ((y*v.z)-(z*v.y)), -((x*v.z)-(z*v.x)), ((x*v.y)-(y*v.x)) );
}

GLfloat Vector3d::length() const
{
    return (sqrt((x*x)+(y*y)+(z*z)));
}

void Vector3d::normalize()
{
    GLfloat tempLength = length();
    x = (x / tempLength);
    y = (y / tempLength);
    z = (z / tempLength);
}

Vector3d Vector3d::operator * (float k) const
{
    GLfloat a = (x * k);
    GLfloat b = (y * k);
    GLfloat c = (z * k);

    return Vector3d {a, b, c};
}

GLfloat Vector3d::getX() const
{
    return x;
}

GLfloat Vector3d::getY() const
{
    return y;
}

GLfloat Vector3d::getZ() const
{
    return z;
}

void Vector3d::setX(GLfloat value)
{
    x = value;
}

void Vector3d::setY(GLfloat value)
{
    y = value;
}

void Vector3d::setZ(GLfloat value)
{
    z = value;
}

void Vector3d::print()
{
        qDebug() << x << " "  << y << " "  << z;
}

// same as the 2d, still ignores height
Vector3d Vector3d::barycentricCoordinates(const Vector3d& dp1, const Vector3d& dp2, const Vector3d& dp3)
{
    // convert 3d to 2d
    Vector2d p1{dp1.x, dp1.z};
    Vector2d p2{dp2.x, dp2.z};
    Vector2d p3{dp3.x, dp3.z};
    Vector2d this3d{this->x, this->z};

    Vector3d baryc; // til retur. Husk

    Vector2d p12 = p2-p1;
    Vector2d p13 = p3-p1;
    Vector3d n = p12^p13;
    float areal_123 = n.length(); // dobbelt areal

    // u
    Vector2d p = (p2 - this3d);
    Vector2d q = (p3 - this3d);
    n = p^q;
    baryc.setX(n.getZ()/areal_123);

    // v
    p = p3 - this3d;
    q = p1 - this3d;
    n = p^q;
    baryc.setY(n.getZ()/areal_123);

    // w
    p = p1 - this3d;
    q = p2 - this3d;
    n = p^q;
    baryc.setZ(n.getZ()/areal_123);

    return baryc;
}
