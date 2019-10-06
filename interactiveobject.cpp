#include "interactiveobject.h"
#include "octahedronball.h"
#include "trianglesurface.h"

InteractiveObject::InteractiveObject(int n) : OctahedronBall(n)
{
mMatrix.scale(ballRadius);
}

void InteractiveObject::move()
{
    // get surface points
    mGround->giveSurfaceTriangleToBall(this);

    // get ground normal
    Vector3d ca = v1-v0;
    Vector3d ba = v2-v0;
    Vector3d currentSurfaceNormal = ca^ba;
    currentSurfaceNormal.normalize();

    // get the barysentric coordinates of the ball on the plane (2d)
    Vector3d baryCoord = position.barycentricCoordinates(v0, v1, v2);

    float groundHeight = (baryCoord.x * v0.y) + (baryCoord.y * v1.y) + (baryCoord.z * v2.y);

    // actually push ball ********************************************

    // move 1 gravity down
//    mMatrix[{1, 3}] = mMatrix[{1, 3}]-0.01f;

    velocity += gravity*timeScale;

    // move by velocity
    position = position + (velocity*timeScale);


    if (position.y <= (groundHeight+ballRadius))
    {
        velocity = -velocity;

        qDebug() << "Ball is on ground";

//        // if under ground -> give ball height of ground
//        mMatrix[{1, 3}] = groundHeight;
//        // + up a bit
//        mMatrix[{1, 3}] = mMatrix[{1, 3}]+(mMatrix[{0, 0}]);

//        // push by ground normal
//        mMatrix[{0, 3}] = mMatrix[{0, 3}] + currentSurfaceNormal.x;
//        mMatrix[{2, 3}] = mMatrix[{2, 3}] + currentSurfaceNormal.z;
    }

    qDebug() << "Simulated time: " << float(myTimer.elapsed()/1000.f) * timeScale;

    mMatrix[{0, 3}] = position.x;
    mMatrix[{1, 3}] = position.y;
    mMatrix[{2, 3}] = position.z;
}

void InteractiveObject::reset()
{
    position = startPos;
    mMatrix[{0, 3}] = position.x;
    mMatrix[{1, 3}] = position.y;
    mMatrix[{2, 3}] = position.z;

    // assuming we don't want an initial velocity
    velocity = Vector3d{0,0,0};
}

void InteractiveObject::setSize(float size)
{
    ballRadius = size;
    mMatrix.scale(ballRadius);
}
