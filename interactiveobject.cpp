#include "interactiveobject.h"
#include "octahedronball.h"
#include "trianglesurface.h"

InteractiveObject::InteractiveObject(int n) : OctahedronBall(n)
{

}

void InteractiveObject::move()
{
    mGround->setBallHeight(this);
}

void InteractiveObject::reset()
{
    mMatrix[{0, 3}] = startPos.x;
            mMatrix[{1, 3}] = startPos.y;
            mMatrix[{2, 3}] = startPos.z;
}
