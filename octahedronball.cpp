#include "octahedronball.h"
#include <iostream>
#include <sstream>
#include <QDebug>
#include <cmath>

OctahedronBall::OctahedronBall(int n) : m_rekursjoner(n), m_indeks(0), VisualObject()
{
    mCollisionObject = new CollisionSphere(this);

    mVertices.reserve(3 * 8 * pow(4, m_rekursjoner));
    oktaederUnitBall();
}

OctahedronBall::~OctahedronBall(){}

void OctahedronBall::lagTriangel(const Vector3d& v1, const Vector3d& v2, const Vector3d& v3)
{
    Vertex v{};

    v.setPosition(v1.getX(), v1.getY(), v1.getZ()); // koordinater v.x = v1.x, v.y=v1.y, v.z=v1.z
    v.setNormals(v1.getX(), v1.getY(), v1.getZ());  // rgb
    v.setUV(0.0f, 0.0f);    // kan utelates
    mVertices.push_back(v);
    v.setPosition(v2.getX(), v2.getY(), v2.getZ());
    v.setNormals(v2.getX(), v2.getY(), v2.getZ());
    v.setUV(1.0f, 0.0f);
    mVertices.push_back(v);
    v.setPosition(v3.getX(), v3.getY(), v3.getZ());
    v.setNormals(v3.getX(), v3.getY(), v3.getZ());
    v.setUV(0.5f, 1.0f);
    mVertices.push_back(v);
}

void OctahedronBall::subDivide(const Vector3d& a, const Vector3d& b, const Vector3d& c, int n)
{
    if (n>0)
    {
        Vector3d v1 = a+b; v1.normalize();
        Vector3d v2 = a+c; v2.normalize();
        Vector3d v3 = c+b; v3.normalize();
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    }
    else
    {
        lagTriangel(a, b, c);
    }
}

void OctahedronBall::oktaederUnitBall()
{
    Vector3d v0{0, 0, 1};
    Vector3d v1{1, 0, 0};
    Vector3d v2{0, 1, 0};
    Vector3d v3{-1, 0, 0};
    Vector3d v4{0, -1, 0};
    Vector3d v5{0, 0, -1};

    subDivide(v0, v1, v2, m_rekursjoner);
    subDivide(v0, v2, v3, m_rekursjoner);
    subDivide(v0, v3, v4, m_rekursjoner);
    subDivide(v0, v4, v1, m_rekursjoner);
    subDivide(v5, v2, v1, m_rekursjoner);
    subDivide(v5, v3, v2, m_rekursjoner);
    subDivide(v5, v4, v3, m_rekursjoner);
    subDivide(v5, v1, v4, m_rekursjoner);
}

void OctahedronBall::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3nd attribute buffer : UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void OctahedronBall::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{
    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }

    //mMatrix.translate(mVelocity);
    mVelocity = mVelocity/1.09f;
    mMatrix[{0,3}] = mMatrix[{0,3}] + mVelocity.x;
    mMatrix[{1,3}] = mMatrix[{1,3}] + mVelocity.y;
    mMatrix[{2,3}] = mMatrix[{2,3}] + mVelocity.z;


    //mRotation.rotate(mVelocity.x, Vector3d{0, 0, 1});
    //mMatrix = mMatrix * mRotation;
    //mRotation.rotate(mVelocity.z, Vector3d{1, 0, 0});
    //tempMat = mRotation * tempMat;

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
}

void OctahedronBall::rotateWhenMove(GLfloat dt)
{

}


