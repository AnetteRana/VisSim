#ifndef INTERACTIVEOBJECT_H
#define INTERACTIVEOBJECT_H

#include "OctahedronBall.h"

class InteractiveObject : public OctahedronBall
{
public:
    InteractiveObject(int n);

    void interactiveMove(GLfloat x = .0f, GLfloat y = .0f, GLfloat z = .0f);
    //void draw() override;
};

#endif // INTERACTIVEOBJECT_H
