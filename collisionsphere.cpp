#include "collisionsphere.h"
#include "octahedronball.h"
#include "VisualObject.h"

CollisionSphere::CollisionSphere(VisualObject* owner)
{
    mOwner = owner;
}

CollisionSphere::CollisionSphere(VisualObject* owner, GLfloat radius)
{
    mOwner = owner;
    mRadius = radius;
}


bool CollisionSphere::detectOverlap(OctahedronBall* other)
{
    GLfloat distanceAtWhichCollisionHappens;
    GLfloat distance;
    if (mRadius >0)
    {
        Vector3d positionA{mOwner->mMatrix[{0, 3}], mOwner->mMatrix[{1, 3}], mOwner->mMatrix[{2, 3}]};
        Vector3d positionB{other->mMatrix[{0, 3}], other->mMatrix[{1, 3}], other->mMatrix[{2, 3}]};
        GLfloat radiusA = mRadius;
        GLfloat radiusB{other->mMatrix[{1, 1}]};

        distanceAtWhichCollisionHappens = radiusA+radiusB;
        Vector3d distanceVector = (positionA - positionB);
        distance = distanceVector.length();
    }
    else
    {
        Vector3d positionA{mOwner->mMatrix[{0, 3}], mOwner->mMatrix[{1, 3}], mOwner->mMatrix[{2, 3}]};
        Vector3d positionB{other->mMatrix[{0, 3}], other->mMatrix[{1, 3}], other->mMatrix[{2, 3}]};
        GLfloat radiusA{mOwner->mMatrix[{1, 1}]};
        GLfloat radiusB{other->mMatrix[{1, 1}]};

        distanceAtWhichCollisionHappens = radiusA+radiusB;
        Vector3d distanceVector = (positionA - positionB);
        distance = distanceVector.length();
    }


    // if collision (should be another function)
    if (distance <= distanceAtWhichCollisionHappens)
    {
        GLfloat speed;
        speed = other->mVelocity.length();
        speed += mOwner->mVelocity.length();
        speed = speed/2;

        Vector3d newVelocityForOther;
        Vector3d newVelocityForOwner;


        newVelocityForOwner.x = mOwner->mMatrix[{0,3}]-other->mMatrix[{0,3}];
        newVelocityForOwner.z = mOwner->mMatrix[{2,3}]-other->mMatrix[{2,3}];

        newVelocityForOther.x += other->mMatrix[{0,3}]-mOwner->mMatrix[{0,3}];
        newVelocityForOther.z += other->mMatrix[{2,3}]-mOwner->mMatrix[{2,3}];

        newVelocityForOwner.normalize();
        newVelocityForOwner = newVelocityForOwner * speed;

        newVelocityForOther.normalize();
        newVelocityForOther = newVelocityForOther * speed;

        mOwner->mVelocity += newVelocityForOwner;
        other->mVelocity += newVelocityForOther;

        return true;
    }
    return false;
}


bool CollisionSphere::detectOverlap(VisualObject* other)
{
    if(dynamic_cast<OctahedronBall*>(other))
       return detectOverlap(dynamic_cast<OctahedronBall*>(other));

    //qDebug() << "CollisionSphere::detectOverlap BANG";

    Vector3d myCenter{mOwner->mMatrix[{0, 3}], mOwner->mMatrix[{1, 3}], mOwner->mMatrix[{2, 3}]};
    GLfloat radius{mOwner->mMatrix[{1, 1}]};

    Vector3d closestPointOnBox;

    // get closest x
    if (myCenter.x < other->mCollisionObject->minXYZ.x)
        closestPointOnBox.x = other->mCollisionObject->minXYZ.x;
    else if (myCenter.x > other->mCollisionObject->maxXYZ.x)
        closestPointOnBox.x = other->mCollisionObject->maxXYZ.x;
    else
        closestPointOnBox.x = myCenter.x;

    // get closest y
    if (myCenter.y < other->mCollisionObject->minXYZ.y)
        closestPointOnBox.y = other->mCollisionObject->minXYZ.y;
    else if (myCenter.y > other->mCollisionObject->maxXYZ.y)
        closestPointOnBox.y = other->mCollisionObject->maxXYZ.y;
    else
        closestPointOnBox.y = myCenter.y;

    // get closest z
    if (myCenter.z < other->mCollisionObject->minXYZ.z)
        closestPointOnBox.z = other->mCollisionObject->minXYZ.z;
    else if (myCenter.z > other->mCollisionObject->maxXYZ.z)
        closestPointOnBox.z = other->mCollisionObject->maxXYZ.z;
    else
        closestPointOnBox.z = myCenter.z;

    Vector3d distanceVector = (closestPointOnBox - myCenter);
    GLfloat distance = distanceVector.length();

    // if collision (should be another function)
    if (distance <= radius)
    {


        if (other->mCollisionObject->isTrigger)
        {
            return true;
        }
        else
        {
            if ((myCenter.z < other->mCollisionObject->maxXYZ.z) && (myCenter.z > other->mCollisionObject->minXYZ.z))
                mOwner->mVelocity.x = -mOwner->mVelocity.x;

            if ((myCenter.x < other->mCollisionObject->maxXYZ.x) && (myCenter.x > other->mCollisionObject->minXYZ.x))
                mOwner->mVelocity.z = -mOwner->mVelocity.z;
            return true;
        }

    }

    return false;
}


