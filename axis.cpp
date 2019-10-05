#include "axis.h"

Axis::Axis()
{
    Vector3d x_axis{1.f, 0.f, 0.f};
    Vector3d y_axis{0.f, 1.f, 0.f};
    Vector3d z_axis{0.f, 0.f, 1.f};

    mVertices.push_back(Vertex{Vector3d{0.f,0.f,0.f}, x_axis});
    mVertices.push_back(Vertex{x_axis, x_axis});

    mVertices.push_back(Vertex{Vector3d{0.f,0.f,0.f}, y_axis});
    mVertices.push_back(Vertex{y_axis, y_axis});

    mVertices.push_back(Vertex{Vector3d{0.f,0.f,0.f}, z_axis});
    mVertices.push_back(Vertex{z_axis, z_axis});
}

void Axis::init(GLint matrixUniform)
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

void Axis::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{
    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glDrawArrays(GL_LINES, 0, mVertices.size());
}
