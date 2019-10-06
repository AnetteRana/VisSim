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

    // simulation time
    QElapsedTimer myTimer;
    float timeScale = .01f;

    // simulation stuff?
    Vector3d position{0,0,0};
    Vector3d velocity{0,0,0};
    Vector3d gravity{0, -9.8f, 0};

    // info about current surface
    Vector3d v0;
    Vector3d v1;
    Vector3d v2;
    Vector3d currentSurfaceNormal;

    // scale
    float ballRadius{1.f};
    void setSize(float);

    //void draw() override;
};

#endif // INTERACTIVEOBJECT_H
