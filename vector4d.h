#ifndef VECTOR4D_H
#define VECTOR4D_H

#include <QOpenGLFunctions_4_1_Core>
#include <cmath>
#include "vector3d.h"


class Vector4d
{
public:
    GLfloat x{0};
    GLfloat y{0};
    GLfloat z{0};
    GLfloat w{0};
public:
    Vector4d(Vector3d v);
    Vector4d(GLfloat x=.0f, GLfloat y=.0f, GLfloat z=.0f, GLfloat w=.0f);

    void print();

    Vector3d getVector3d();
    GLfloat get(int index) const;

    GLfloat& operator[] (const int index);
};

#endif // VECTOR4D_H
