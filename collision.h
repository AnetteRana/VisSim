#ifndef COLLISION_H
#define COLLISION_H

#include "matrix4x4.h"
#include <QMatrix4x4>
#include "shader.h"
#include "VisualObject.h"
#include "trianglesurface.h"

class OctahedronBall;
class VisualObject;

class CollisionObject : public Matrix4x4 , protected QOpenGLFunctions_4_1_Core
{
public:
    //CollisionObject();
    virtual bool detectOverlap(OctahedronBall* other)=0;
    virtual bool detectOverlap(VisualObject* other)=0;

    // only relevant for collisionboxaabb
    Vector3d maxXYZ{0,0,0};
    Vector3d minXYZ{0,0,0};

    bool isTrigger{false};
    GLfloat mRadius{0};

};

#endif // COLLISION_H
