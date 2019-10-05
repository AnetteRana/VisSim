#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <QOpenGLFunctions_4_1_Core>
#include "vector3d.h"

class Vector2d
{
public:
    GLfloat x{0};
    GLfloat y{0};
public:
    Vector2d(GLfloat);
    Vector2d(GLfloat x=.0f, GLfloat y = .0f);
    GLfloat getX() const;
    GLfloat getY() const;
    void setX(GLfloat value);
    void setY(GLfloat value);

    Vector2d& operator = (const Vector2d& v);
    Vector2d operator + (const Vector2d& v) const;
    Vector2d operator * (const GLfloat& v) const;
    Vector2d operator - (const Vector2d& v) const;
    Vector3d operator ^ (const Vector2d& v) const;      // Cross product
    Vector3d barycentricCoordinates(const Vector2d& p1, const Vector2d& p2, const Vector2d& p3);
    Vector2d baryNeighbour(const Vector2d& p1, const Vector2d& p2, const Vector2d& p3);

    void print();

};

#endif // VECTOR2D_H
