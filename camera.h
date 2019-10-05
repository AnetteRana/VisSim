#ifndef CAMERA_H
#define CAMERA_H

#include "matrix4x4.h"
#include <QMatrix4x4>
#include "shader.h"
#include "VisualObject.h"

class Camera : public Matrix4x4 , protected QOpenGLFunctions_4_1_Core
{
public:
    Camera(Vector3d position = Vector3d(0, 0, 0), bool useLookAt = false);

    void init(Shader* shader);
    void lookAt();
    void frustum();
    void perspective(GLfloat aspectRatio);
    void update();


    Vector3d mPosition;
    QMatrix4x4 *mVMatrix; // view
    QMatrix4x4 *mPMatrix; // projeksjonsmatrise


    bool useLookAt{false};
    VisualObject* mTarget{nullptr};
    void setLookAtTarget(VisualObject* target);



    //Shader *mShaderProgram;
    GLint  mPMatrixUniform{0};
    GLint  mVMatrixUniform{0};

};

#endif // CAMERA_H

