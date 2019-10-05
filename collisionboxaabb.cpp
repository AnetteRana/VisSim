#include "collisionboxaabb.h"

CollisionBoxAABB::CollisionBoxAABB(){}

CollisionBoxAABB::CollisionBoxAABB(Vector3d max, Vector3d min)
{
    maxXYZ = max;
    minXYZ = min;
}

CollisionBoxAABB::CollisionBoxAABB(Vector3d max, Vector3d min, bool is_trigger)
{
    maxXYZ = max;
    minXYZ = min;
    isTrigger = is_trigger;
}

bool CollisionBoxAABB::detectOverlap(VisualObject* other)
{
    int counter=0;

    if ((minXYZ.x <= other->mCollisionObject->maxXYZ.x) && (maxXYZ.x >= other->mCollisionObject->minXYZ.x))
    {
        //qDebug() << "overlap in x";
        counter++;
    }

    if ((minXYZ.y <= other->mCollisionObject->maxXYZ.y) && (maxXYZ.y >= other->mCollisionObject->minXYZ.y))
    {
        //qDebug() << "overlap in y";
        counter++;
    }

    if ((minXYZ.z <= other->mCollisionObject->maxXYZ.z) && (maxXYZ.z >= other->mCollisionObject->minXYZ.z))
    {
        //qDebug() << "overlap in z";
        counter++;
    }

    if (counter == 3)
        return true;
    else {
        return false;
    }
}

bool CollisionBoxAABB::detectOverlap(OctahedronBall* other)
{
    return false;
}
