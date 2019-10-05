#ifndef PLANE_H
#define PLANE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "vector3d.h"
#include "VisualObject.h"

class Plane : public VisualObject
{
public:
    Plane(int sections = 1, GLfloat width = 1.f);
    Plane(int sections, GLfloat width, GLfloat height);

    void init(GLint matrixUniform) override;
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) override;
};

#endif // PLANE_H
