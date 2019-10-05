#ifndef GRAPH3D_H
#define GRAPH3D_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "vector3d.h"
#include "VisualObject.h"

class Graph3d : public VisualObject
{
public:
    Graph3d(int sections = 2, GLfloat width = 1.f);

    // silly stuff
    void makeMesh();
    GLfloat time{0};
    int mSections;
    GLfloat mWidth;


    void init(GLint matrixUniform) override;
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) override;

    GLfloat getFunctionHeight(GLfloat, GLfloat);
    GLfloat getNormalX(GLfloat);

    void calculateNormals();


};

#endif // GRAPH3D_H
