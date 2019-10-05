#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "VisualObject.h"
#include "vector3d.h"

class Tetrahedron : public VisualObject
{
public:
    Tetrahedron(GLfloat width);

    void init(GLint matrixUniform) override;
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) override;
};

#endif // TETRAHEDRON_H
