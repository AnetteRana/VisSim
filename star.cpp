#include "star.h"

Star::Star ()
{

    GLfloat width = .1f;

    // colors
    Vector3d center{0, 0, 0};
    Vector3d a{2*width, 2*width, 0};
    Vector3d b{3*width, -1*width, 0};
    Vector3d c{0*width, -3*width, 0};
    Vector3d d{-3*width, -1*width, 0};
    Vector3d e{-2*width, 2*width, 0};
    Vector3d f, g,h,i,j{0,0,0};



    f = (a-b)*1.1f;
    g = (b-c)*1.1f;
    h = (c-d)*1.1f;
    i = (d-e)*1.1f;
    j = (e-a)*1.1f;

    // points
    Vertex n{center, mColor};

    Vertex A{a, mColor};
    Vertex B{b, mColor};
    Vertex C{c, mColor};
    Vertex D{d, mColor};
    Vertex E{e, mColor};

    Vertex F{f, mColor};
    Vertex G{g, mColor};
    Vertex H{h, mColor};
    Vertex I{i, mColor};
    Vertex J{j, mColor};



    // triangle 1
    mVertices.push_back(A);
    mVertices.push_back(B);
    mVertices.push_back(n);

    // triangle 2
    mVertices.push_back(B);
    mVertices.push_back(C);
    mVertices.push_back(n);

    // triangle 3
    mVertices.push_back(C);
    mVertices.push_back(D);
    mVertices.push_back(n);

    // triangle 4
    mVertices.push_back(D);
    mVertices.push_back(E);
    mVertices.push_back(n);

    // triangle 5
    mVertices.push_back(E);
    mVertices.push_back(A);
    mVertices.push_back(n);

    // triangle 6 - a point between a and b
    mVertices.push_back(A);
    mVertices.push_back(B);
    mVertices.push_back(F);
    // triangle
    mVertices.push_back(B);
    mVertices.push_back(C);
    mVertices.push_back(G);
    // triangle
    mVertices.push_back(C);
    mVertices.push_back(D);
    mVertices.push_back(H);
    // triangle
    mVertices.push_back(D);
    mVertices.push_back(E);
    mVertices.push_back(I);
    // triangle
    mVertices.push_back(E);
    mVertices.push_back(A);
    mVertices.push_back(J);

}

void Star::setColor(Vector3d color)
{
    mColor = color;

    for (auto it = mVertices.begin(); it != mVertices.end(); it++)
    {
        (*it).setNormals(mColor.getX(), mColor.getY(), mColor.getZ());
    }
}

void Star::init(GLint matrixUniform)
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

void Star::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{
    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }

    //initializeOpenGLFunctions();
    //move(.1f, 0, 0);
    //mMatrix.translate(sin(animate)/10, 0, 0);
    // animate
    //mMatrix.rotate(.5f, 0, 1, 0);

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, signed(mVertices.size()));
}

