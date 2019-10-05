#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "VisualObject.h"
#include "vector3d.h"
#include "plane.h"
#include "graph3d.h"

class Cube : public VisualObject
{
private:
    void pushVertices(Vertex A, Vertex B, Vertex C, Vertex D, Vertex E, Vertex F, Vertex G, Vertex H);
public:
    Cube(GLfloat width = 1.f);
    Cube(Vector3d max, Vector3d min);
    Cube(Vector3d max, Vector3d min, bool isVisible, bool hasCollision);


    void init(GLint matrixUniform) override;
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) override;
};

#endif // CUBE_H
