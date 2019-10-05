#ifndef COLLISIONSPHERE_H
#define COLLISIONSPHERE_H

#include "collision.h"
#include "collisionboxaabb.h"

class OctahedronBall;

class CollisionSphere : public CollisionObject
{
public:
    CollisionSphere(VisualObject* owner);
    CollisionSphere(VisualObject* owner, GLfloat radius);

    bool detectOverlap(VisualObject* other) override;
    bool detectOverlap(OctahedronBall* other) override;

    VisualObject* mOwner{nullptr};


};

#endif // COLLISIONSPHERE_H
