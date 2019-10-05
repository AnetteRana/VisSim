#ifndef OBJMESH_H
#define OBJMESH_H
#include "visualobject.h"


class ObjMesh : public VisualObject
{
public:
    ObjMesh();
    ObjMesh(std::string objectName, std::string filename, Vector3d inTrans, Vector3d inRot, float rotAmount, Vector3d inScale);

    ~ObjMesh() override;

    void readFile(std::string filename);

    void init(GLint matrixUniform) override;
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) override;

    void move(float x, float y, float z, Vector3d surfNormal);
    void rotate(float amount, int x, int y, int z);
    void setLocation(float inX, float inY, float inZ);
    Vector3d getPosition();
    Vector3d override;

    void interactiveMove(GLfloat x, GLfloat y, GLfloat z);

private:
    float mx, my, mz; // position
    float mrx, mry, mrz; // rotation

    GLfloat mMoveSpeed{2};
    float mRotSpeed{2};
};

#endif // OBJMESH_H

