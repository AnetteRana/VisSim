#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "VisualObject.h"

class BezierCurve : public VisualObject
{
public:
    BezierCurve(bool curve = true, bool control = true, GLfloat curveSmoothness = 8.f);
    BezierCurve(Vector3d,Vector3d,Vector3d,Vector3d, bool curve = true, bool control = true, GLfloat curveSmoothness = 8.f);

    void init(GLint matrixUniform) override;
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) override;
    void drawCurve();
    void drawControl();

    GLfloat mCurveSmoothness;
    Vector3d evaluateBezier(int levelOfDetail, GLfloat t);

    Vector3d points[4];
};

#endif // BEZIERCURVE_H
