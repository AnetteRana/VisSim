#include "interactiveobject.h"
#include "octahedronball.h"

InteractiveObject::InteractiveObject(int n) : OctahedronBall(n)
{

}

void InteractiveObject::interactiveMove(GLfloat x, GLfloat y, GLfloat z)
{
    Vector3d addedSpeed {x, y, z};
    //qDebug() << "InteractiveObject::move";
    //mMatrix.translate(x, y, z);
    if (mVelocity.length() <= .5f) // max speed
        mVelocity = mVelocity + addedSpeed;
}
