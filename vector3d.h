#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <QOpenGLFunctions_4_1_Core>
#include <cmath>

class Vector4d;

class Vector3d
{
    friend class Vector4d;
public:
    GLfloat x{0};
    GLfloat y{0};
    GLfloat z{0};
public:
    //Vector3d();
    Vector3d(GLfloat);
    Vector3d(GLfloat x=.0f, GLfloat y = .0f, GLfloat z = .0f);// Constructor
    Vector3d(Vector4d);

    Vector3d operator*(Vector3d);
    Vector3d operator*(GLfloat);
    Vector3d operator/(GLfloat);
    Vector3d operator+(GLfloat);
    Vector3d operator-(GLfloat);


    const Vector3d& operator=(const Vector3d& v);       // Assignment
    const Vector3d& operator+=(const Vector3d& v);
    Vector3d operator + (const Vector3d& v) const;      // Addition
    Vector3d operator - (const Vector3d& v) const;      // Subtraction
    Vector3d operator - () const;                       // negative
    float operator * (const Vector3d& v) const;         // Dot product
    Vector3d operator ^ (const Vector3d& v) const;      // Cross product
    GLfloat length() const;                             // return length
    void normalize();                                   // Normalize to unit length
    Vector3d operator * (float c) const;                // Scaling
    GLfloat getX() const;
    GLfloat getY() const;
    GLfloat getZ() const;
    void setX(GLfloat value);
    void setY(GLfloat value);
    void setZ(GLfloat value);
    void print();
    Vector3d barycentricCoordinates(const Vector3d& dp1, const Vector3d& dp2, const Vector3d& dp3);
};

#endif // VECTOR3D_H
