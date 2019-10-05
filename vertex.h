#ifndef VERTEX_H
#define VERTEX_H

#include <QOpenGLFunctions_4_1_Core>

#include "vector3d.h"
#include "vector2d.h"

class Vertex
{
public:
    Vertex();
    Vertex(Vector3d position);
    Vertex(Vector3d position, Vector3d color);
    Vertex(Vector3d position, Vector3d color, Vector2d UV);

    void print();


    Vector3d mPosition;
    Vector3d mColor;
    Vector2d mTextureCoordinates{0,0};

    void setPosition(GLfloat x, GLfloat y, GLfloat z);
    void setNormals(GLfloat x, GLfloat y, GLfloat z);
    void setUV(GLfloat u, GLfloat v);

private:
    friend std::ostream& operator<< (std::ostream& os, const Vertex& v);
    friend std::istream& operator>> (std::istream& is, Vertex& v);
};




#endif // VERTEX_H
