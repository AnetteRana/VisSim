#ifndef STAR_H
#define STAR_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "vector3d.h"
//#include "interactiveobject.h"
#include "VisualObject.h"


class Star : public VisualObject
{
public:
    Star();

    void init(GLint matrixUniform) override;
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) override;

    GLfloat animate{0};

    void setColor(Vector3d color);
    Vector3d mColor {1.f, 1.f, 1.f};
};

#endif // STAR_H
