#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include <QOpenGLFunctions_4_1_Core>
#include "vector3d.h"
#include "vector4d.h"

class Matrix4x4 //: protected QOpenGLFunctions_4_1_Core
{
protected:
    // data is stored as column-major format...
    GLfloat data[4][4]{0};

    GLfloat detArray[3][3]; // helps to find determinant
public:
    Matrix4x4();
    Matrix4x4(Vector4d a, Vector4d b, Vector4d c, Vector4d d);
    Matrix4x4(GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
              GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
              GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
              GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33 );

    void print();

    // can't use matrix[1][0] but can use matrix[{1, 0}]
    GLfloat& operator[](const std::pair<int,int>& Index);

    // get
    void LU();
    //void pivot(int);
    Vector4d solve(Vector4d &b) const;
    //Matrix4x4 LU() const; //(lage 0 på nedre triangulær AKA gausselim)
    GLfloat determinant();
    GLfloat determinantHelper();

    void setToIdentity();
    void setToZero();
    Matrix4x4 projectionMatrix() const;
    Matrix4x4 inverseMatrix() const;
    Matrix4x4 transposed() const;

    Matrix4x4 operator * (const GLfloat& c) const;
    Vector4d operator * (Vector4d& v) const;
    Matrix4x4 operator * (const Matrix4x4& m) const;
    void scale(GLfloat value);
    void scale(GLfloat x, GLfloat y, GLfloat z);
    void scale(Vector3d& v);
    void rotate(GLfloat angle, Vector3d v);
    void translate(GLfloat x=.0f, GLfloat y=.0f, GLfloat z=.0f);
    void translate(Vector3d& v);
    void lookAt(const Vector3d &eye, const Vector3d &at, const Vector3d &up);
    void frustum(float left, float right, float bottom, float top, float near, float far);

    const GLfloat* constData();

};

#endif // MATRIX3X3_H
