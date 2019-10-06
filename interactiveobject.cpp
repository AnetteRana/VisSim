#include "interactiveobject.h"
#include "octahedronball.h"
#include "trianglesurface.h"
#include "renderwindow.h"

InteractiveObject::InteractiveObject(int n, RenderWindow* owner) : OctahedronBall(n)
{
    mOwner = owner;

    mMatrix.scale(ballRadius);

    setPosition(Vector3d(0,0,0));
    setVelocity(Vector3d(0,0,0));
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

    float deltaTime = float(myTimer.elapsed()/1000.f) - float(lastTimestamp);
    qDebug() << "now time: " << myTimer.elapsed()/1000.f << "last time: " << lastTimestamp << "delta " << deltaTime;
    lastTimestamp = myTimer.elapsed()/1000.f;
    //qDebug() << "Simulated time: " << lastTimestamp/1000.f * timeScale;
    qDebug() << "velocity: ";
                velocity.print();

    velocity += gravity*deltaTime*timeScale;

    // move by velocity
    position = position + (velocity*deltaTime*timeScale);


    // does it hit the ground?
    if (position.y <= (groundHeight+ballRadius))
    {
        //velocity = -velocity ;

        // stop simulation
        mOwner->isSimulating = false;

        qDebug() << "Ball is on ground";


        // if under ground -> give ball height of ground
        if (position.y < (groundHeight+ballRadius))
        {
            mMatrix[{1, 3}] = (groundHeight+ballRadius);
            //velocity += -gravity*timeScale; // no, this should be the normal of the ground?
        }
        //        // + up a bit
        //        mMatrix[{1, 3}] = mMatrix[{1, 3}]+(mMatrix[{0, 0}]);

        //        // push by ground normal
        //        mMatrix[{0, 3}] = mMatrix[{0, 3}] + currentSurfaceNormal.x;
        //        mMatrix[{2, 3}] = mMatrix[{2, 3}] + currentSurfaceNormal.z;
    }


    mMatrix[{0, 3}] = position.x;
    mMatrix[{1, 3}] = position.y;
    mMatrix[{2, 3}] = position.z;
}

void InteractiveObject::reset()
{
    position = initialPosition;
    mMatrix[{0, 3}] = position.x;
    mMatrix[{1, 3}] = position.y;
    mMatrix[{2, 3}] = position.z;

    velocity = initialVelocity;
}

void InteractiveObject::setSize(float size)
{
    ballRadius = size;
    mMatrix.scale(ballRadius);
}

void InteractiveObject::setPosition(Vector3d pos)
{
    initialPosition = pos;
    position = pos;
    mMatrix.translate(pos);
}

void InteractiveObject::setVelocity(Vector3d vel)
{
    initialVelocity = vel;
    velocity = vel;
}
