#ifndef VisualObject_H
#define VisualObject_H

#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "vertex.h"
#include "vector2d.h"
#include "vector3d.h"
#include "vector4d.h"
#include "matrix4x4.h"
#include <qdebug.h>
//#include <QMatrix4x4>
#include "stb_image.h"
#include "shader.h"

class CollisionObject;

class VisualObject : protected QOpenGLFunctions_4_1_Core
{
protected:
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;
    GLuint mVAO;
    GLuint mVBO;
    GLuint mEBO;
    GLint mMatrixUniform;

    unsigned int mTexture;

public:

    Vector3d mRotation{0,0,0};

    GLfloat myRuntime{0};
    std::string textureFileName{"../GSOpenGL2019/textures/missing.jpg"};
    Matrix4x4 mMatrix;
    Vector3d mVelocity{0, 0, 0};

    std::string mName{"unnamed"};
    VisualObject(){}

    virtual void init(GLint matrixUniform){}

    virtual void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) = 0;
    virtual void update(){}

    // collision
    CollisionObject* mCollisionObject{nullptr};

//void AddVerticesFrom(VisualObject* object);

Shader* mShader{nullptr};

};

#endif // VisualObject_H
