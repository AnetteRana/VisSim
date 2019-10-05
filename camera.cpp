#include "camera.h"

Camera::Camera(Vector3d position, bool _useLookAt)
{
    useLookAt = _useLookAt;
    mPosition = position;


    mVMatrix = new QMatrix4x4{};
    mPMatrix = new QMatrix4x4{};

    //    mPMatrix->perspective(60, 4.0f/3.0f, 1.f, -1.f);
    //    //mPMatrix->frustum(-1,1,-1,1,1,-1);
}

void Camera::init(Shader* shader)
{
    initializeOpenGLFunctions();
}

void Camera::perspective(GLfloat aspectRatio)
{
    mPMatrix->setToIdentity();

    mPMatrix->perspective(60, aspectRatio, 1.f, -1.f);
}

void Camera::frustum()
{
    mPMatrix->setToIdentity();
    mPMatrix->frustum(-1,1,-1,1,1,-1);
}

void Camera::lookAt()
{
    QVector3D targetPosition = QVector3D(mTarget->mMatrix[{0, 3}], mTarget->mMatrix[{1, 3}], mTarget->mMatrix[{2, 3}]);

    mVMatrix->setToIdentity();
    QVector3D cameraPosition{mPosition.getX(), mPosition.getY(), mPosition.getZ()};
    //QVector3D targetPosition(myRuntime*2, 0, 0);
    QVector3D up(0, 1, 0);

    mVMatrix->lookAt(cameraPosition, targetPosition, up);
}

void Camera::setLookAtTarget(VisualObject* target)
{
    mTarget = target;
}

void Camera::update()
{
    if (useLookAt && mTarget)
    {
        lookAt();
    }
//    else
//    {
//        mVMatrix->setToIdentity();
//        QVector3D cameraPosition{mPosition.getX(), mPosition.getY(), mPosition.getZ()};
//    }
}
