#ifndef OCTAHEDRONBALL_H
#define OCTAHEDRONBALL_H
#include "VisualObject.h"
#include "collisionsphere.h"
#include "collision.h"

//class CollisionSphere;

class OctahedronBall : public VisualObject
{
private:
    int m_rekursjoner;
    int m_indeks;               // brukes i rekursjon, til å bygge m_vertices
    void lagTriangel(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3);
    void subDivide(const Vector3d& a, const Vector3d& b, const Vector3d& c, int n);
    void oktaederUnitBall();

public:
    OctahedronBall(int n=0);
    ~OctahedronBall();
    void init(GLint matrixUniform);
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource);
    void rotateWhenMove(GLfloat dt);




};

#endif // OCTAHEDRONBALL_H
