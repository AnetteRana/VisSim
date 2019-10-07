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
    isOnGround = mGround->giveSurfaceTriangleToBall(this);

    // if out of bounds
    if (isOnGround != true)
    {
        if (abs(position.x) > mGround->radius)
            velocity.x = -velocity.x;
        if (abs(position.z) > mGround->radius)
            velocity.z = -velocity.z;

        //            if (mOwner->isSimulating)
        //            {
        //                mOwner->toggleSimulation();
        //                infoPrinting();
        //            }
    }

    // get the barysentric coordinates of the ball on the plane (2d)
    Vector3d baryCoord = position.barycentricCoordinates(v0, v1, v2);

    float groundHeight = (baryCoord.x * v0.y) + (baryCoord.y * v1.y) + (baryCoord.z * v2.y);

    // update time
    float deltaTime = float(myTimer.elapsed()/1000.f) - float(lastTimestamp); // find deltaTime
    lastTimestamp = myTimer.elapsed()/1000.f;

    // actually push ball ********************************************
    // move by velocity
    position += velocity*deltaTime*timeScale;

    // does it hit the ground?
    if (position.y <= (groundHeight+ballRadius))
    {
        // stop simulation on impact?
        if (false)
        {
            if (mOwner->isSimulating)
            {
                mOwner->toggleSimulation();
                infoPrinting();
            }
        }

        //qDebug() << "Ball is on ground";

        // get surface normal
        Vector3d ca = v1-v0;
        Vector3d ba = v2-v0;
        Vector3d currentSurfaceNormal = ca^ba;
        currentSurfaceNormal.normalize();

        // roll along surface
        //velocity += (currentSurfaceNormal+gravity) *(deltaTime*timeScale*bounciness);
        velocity += (currentSurfaceNormal*velocity.length()) *(deltaTime*timeScale*bounciness);
    }
    else // freefall
    {
        velocity += gravity*deltaTime*timeScale;
    }

    // if under ground -> give ball height of ground
    if (position.y < (groundHeight+ballRadius))
    {
        position.setY((groundHeight+ballRadius));
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

void InteractiveObject::infoPrinting()
{
    qDebug() << "Simulation time:" << myTimer.elapsed()/1000.f << "seconds (realtime)";
    qDebug() << "Simulated seconds:" << timeScale*myTimer.elapsed()/1000.f;
    qDebug() << "velocity:";
    velocity.print();
    Vector3d distanceTravelled = initialPosition - position;
    qDebug() << "distance:";
    distanceTravelled.print();
}
