#ifndef GRAPH2D_H
#define GRAPH2D_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "vector3d.h"
#include "VisualObject.h"

class Graph2d : public VisualObject
{
public:
    Graph2d();
    GLfloat SomeFunction(GLfloat);

    void init(GLint matrixUniform) override;
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) override;
};

#endif // GRAPH2D_H
