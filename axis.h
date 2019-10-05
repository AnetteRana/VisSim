#ifndef AXIS_H
#define AXIS_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "VisualObject.h"
#include "vector3d.h"
#include "vertex.h"


class Axis : public VisualObject
{
public:
    Axis();

    void init(GLint matrixUniform) override;
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) override;
};

#endif // AXIS_H
