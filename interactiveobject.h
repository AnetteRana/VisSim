#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "OctahedronBall.h"
#include <QElapsedTimer>

class RenderWindow;

class TriangleSurface;

class InteractiveObject : public OctahedronBall
{
public:
    InteractiveObject(int n, RenderWindow*);

    void move();

    TriangleSurface* mGround{nullptr};

    RenderWindow* mOwner{nullptr};

    void reset();

    // simulation time
    QElapsedTimer myTimer;
    float timeScale = 1.f;
    float lastTimestamp{0};

    // simulation stuff?
    Vector3d initialVelocity{0,0,0};
    Vector3d initialPosition{0,0,0};
    Vector3d position{0,0,0};
    Vector3d velocity{0,0,0};
    Vector3d gravity{0, -9.8f, 0};

    void infoPrinting();

    // info about current surface
    Vector3d v0;
    Vector3d v1;
    Vector3d v2;
    Vector3d currentSurfaceNormal;

    // scale
    float ballRadius{1.f};
    void setSize(float);

    void setPosition(Vector3d);
    void setVelocity(Vector3d);

    //void draw() override;

};

#endif // INTERACTIVEOBJECT_H
