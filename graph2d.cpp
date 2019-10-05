#include "graph2d.h"
#include <math.h>

Graph2d::Graph2d()
{
    int howManyPointsOnX{100};

    float x{0};
    float y{0};
    float stride = (2.f/howManyPointsOnX);

    for (int p = 0; p <= howManyPointsOnX; p++)
    {
        x = ((p * stride) - 1);
        y = SomeFunction(x);
        mVertices.push_back(Vertex{Vector3d{x, y, 0.0f},     Vector3d{1.f, 1.0f, 1.f}});
    }
}

GLfloat Graph2d::SomeFunction(GLfloat x)
{
    GLfloat y = sin(x*100);
    return y;
}

void Graph2d::init(GLint matrixUniform)
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

void Graph2d::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{
    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glDrawArrays(GL_LINE_STRIP, 0, signed(mVertices.size()));
}
