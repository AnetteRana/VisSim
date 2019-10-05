#ifndef RECURSIVETRIANGLES_H
#define RECURSIVETRIANGLES_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "VisualObject.h"
#include "vector3d.h"
#include "plane.h"
#include "graph3d.h"

class RecursiveTriangles : public VisualObject
{
public:
    RecursiveTriangles(GLfloat radius = 1.f, GLint recursions = 2);
    GLfloat myRuntime{0};

    void init(GLint matrixUniform) override;
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) override;
};

#endif // RECURSIVETRIANGLES_H
