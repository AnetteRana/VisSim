#include "tetrahedron.h"

Tetrahedron::Tetrahedron(GLfloat width)
{
    Vector3d color_A {0.976f, 0.859f, 0.741f};
    Vector3d color_B ( 0.565f, 0.298f, 0.467f);
    Vector3d color_C (0.961f, 0.392f, 0.663f);

    Vertex A{Vector3d{width, width, width},      color_C};
    Vertex B{Vector3d{width, -width, -width},    color_C};
    Vertex C{Vector3d{-width, width, -width},    color_A};
    Vertex D{Vector3d{-width, -width, width},    color_A};

    // triangle 1
    mVertices.push_back(A);
    mVertices.push_back(B);
    mVertices.push_back(C);

    // triangle 2
    mVertices.push_back(A);
    mVertices.push_back(D);
    mVertices.push_back(B);

    // triangle 3
    mVertices.push_back(B);
    mVertices.push_back(D);
    mVertices.push_back(C);

    // triangle 4
    mVertices.push_back(A);
    mVertices.push_back(C);
    mVertices.push_back(D);

    mMatrix.scale(1,1,1);
    mMatrix.rotate(20, Vector3d{1, 0, 0});
}

void Tetrahedron::init(GLint matrixUniform)
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

void Tetrahedron::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{
    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, signed(mVertices.size()));
}
