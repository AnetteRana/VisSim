#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "OctahedronBall.h"
#include <QElapsedTimer>

class TriangleSurface;

class InteractiveObject : public OctahedronBall
{
public:
    InteractiveObject(int n);

    void move();

    TriangleSurface* mGround{nullptr};

    Vector3d startPos{0,0,0};

    void reset();

    QElapsedTimer myTimer;

    // info about current surface
    Vector3d v0;
    Vector3d v1;
    Vector3d v2;
    Vector3d currentSurfaceNormal;

    //void draw() override;
};

#endif // INTERACTIVEOBJECT_H
