#include "vector4d.h"
#include "qdebug.h"

Vector4d::Vector4d(Vector3d v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = 0;
}

Vector4d::Vector4d(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
    x = a;
    y = b;
    z = c;
    w = d;
}

void Vector4d::print()
{
        qDebug() << x << " "  << y << " "  << z << " "  << w;
}

Vector3d Vector4d::getVector3d()
{
    return Vector3d(x, y, z);
}

GLfloat Vector4d::get(int index) const
{
    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        return (-1);
    }
}

GLfloat& Vector4d::operator[] (const int index)
{
    switch (index)
    {
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    default:
        qDebug() <<  "Out of bounds index in vector4d ";
        return w;
    }
}
