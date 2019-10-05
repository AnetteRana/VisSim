#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "OctahedronBall.h"

class TriangleSurface;

class InteractiveObject : public OctahedronBall
{
public:
    InteractiveObject(int n);

    void move();

    TriangleSurface* mGround{nullptr};

    Vector3d startPos{0,0,0};

    void reset();

    //void draw() override;
};

#endif // INTERACTIVEOBJECT_H
