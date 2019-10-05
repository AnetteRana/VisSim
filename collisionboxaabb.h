#ifndef COLLISIONBOXAABB_H
#define COLLISIONBOXAABB_H


#include "collision.h"

class CollisionBoxAABB : public CollisionObject
{
public:
    CollisionBoxAABB();
    CollisionBoxAABB(Vector3d max, Vector3d min);
    CollisionBoxAABB(Vector3d max, Vector3d min, bool is_trigger);

    bool detectOverlap(VisualObject* other) override;
    bool detectOverlap(OctahedronBall* other) override;


};

#endif // COLLISIONBOXAABB_H
