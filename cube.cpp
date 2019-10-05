#include "cube.h"
#include "collisionboxaabb.h"


Cube::Cube(GLfloat width)
{

    Vector3d color {0.76f, 0.59f, 0.41f};

    Vertex A{Vector3d{width, width, width},         Vector3d{1,0,0}};
    Vertex B{Vector3d{width, width, -width},        Vector3d{1,0,0}};
    Vertex C{Vector3d{width, -width, width},        Vector3d{1,0,0}};
    Vertex D{Vector3d{width, -width, -width},       Vector3d{1,0,0}};
    Vertex E{Vector3d{-width, width, width},        Vector3d{1,0,0}};
    Vertex F{Vector3d{-width, width, -width},       Vector3d{1,0,0}};
    Vertex G{Vector3d{-width, -width, width},       Vector3d{1,0,0}};
    Vertex H{Vector3d{-width, -width, -width},      Vector3d{1,0,0}};

pushVertices(A, B, C, D, E, F, G, H);

}

Cube::Cube(Vector3d max, Vector3d min)
{

    Vector3d color {0.76f, 0.59f, 0.41f};

    Vertex A{Vector3d{max.x, max.y, max.z},     Vector3d{1,0,0}};
    Vertex B{Vector3d{max.x, max.y, min.z},     Vector3d{1,0,0}};
    Vertex C{Vector3d{max.x, min.y, max.z},     Vector3d{1,0,0}};
    Vertex D{Vector3d{max.x, min.y, min.z},     Vector3d{1,0,0}};
    Vertex E{Vector3d{min.x, max.y, max.z},     Vector3d{1,0,0}};
    Vertex F{Vector3d{min.x, max.y, min.z},     Vector3d{1,0,0}};
    Vertex G{Vector3d{min.x, min.y, max.z},     Vector3d{1,0,0}};
    Vertex H{Vector3d{min.x, min.y, min.z},     Vector3d{1,0,0}};

    pushVertices(A, B, C, D, E, F, G, H);

}

Cube::Cube(Vector3d max, Vector3d min, bool isVisible, bool hasCollision)
{
    if (hasCollision)
    {
        mCollisionObject = new CollisionBoxAABB(max, min);
    }

    if (isVisible)
    {
        Vector3d color {0,0,0};

        Vertex A{Vector3d{max.x, max.y, max.z},     color};
        Vertex B{Vector3d{max.x, max.y, min.z},     color};
        Vertex C{Vector3d{max.x, min.y, max.z},     color};
        Vertex D{Vector3d{max.x, min.y, min.z},     color};
        Vertex E{Vector3d{min.x, max.y, max.z},     color};
        Vertex F{Vector3d{min.x, max.y, min.z},     color};
        Vertex G{Vector3d{min.x, min.y, max.z},     color};
        Vertex H{Vector3d{min.x, min.y, min.z},     color};

        pushVertices(A, B, C, D, E, F, G, H);
    }
}

void Cube::pushVertices(Vertex A, Vertex B, Vertex C, Vertex D, Vertex E, Vertex F, Vertex G, Vertex H)
{
    // triangle 1
    mVertices.push_back(A);
    mVertices.push_back(B);
    mVertices.push_back(C);

    mVertices.push_back(B);
    mVertices.push_back(D);
    mVertices.push_back(C);

        for (auto it = mVertices.end()-6; it != mVertices.end(); it++)
        {
            (*it).setNormals(1,0,0);
        }

    //
    mVertices.push_back(E);
    mVertices.push_back(A);
    mVertices.push_back(G);

    mVertices.push_back(A);
    mVertices.push_back(C);
    mVertices.push_back(G);

    for (auto it = mVertices.end()-6; it != mVertices.end(); it++)
    {
        (*it).setNormals(0,0,1);
    }

    //
    mVertices.push_back(F);
    mVertices.push_back(E);
    mVertices.push_back(H);

    mVertices.push_back(E);
    mVertices.push_back(G);
    mVertices.push_back(H);

    for (auto it = mVertices.end()-6; it != mVertices.end(); it++)
    {
        (*it).setNormals(-1,0,0);
    }

    //
    mVertices.push_back(B);
    mVertices.push_back(F);
    mVertices.push_back(D);

    mVertices.push_back(F);
    mVertices.push_back(H);
    mVertices.push_back(D);

    for (auto it = mVertices.end()-6; it != mVertices.end(); it++)
    {
        (*it).setNormals(0,0,-1);
    }

    //
    mVertices.push_back(E);
    mVertices.push_back(F);
    mVertices.push_back(A);

    mVertices.push_back(F);
    mVertices.push_back(B);
    mVertices.push_back(A);

    for (auto it = mVertices.end()-6; it != mVertices.end(); it++)
    {
        (*it).setNormals(0,1,0);
    }

    //
    mVertices.push_back(C);
    mVertices.push_back(D);
    mVertices.push_back(G);

    mVertices.push_back(D);
    mVertices.push_back(H);
    mVertices.push_back(G);

    for (auto it = mVertices.end()-6; it != mVertices.end(); it++)
    {
        (*it).setNormals(0,-1,0);
    }
}

void Cube::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3nd attribute buffer : UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Cube::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{
    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }

    //mMatrix.rotate(1, Vector3d{1,0,0});
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, signed(mVertices.size()));
}
