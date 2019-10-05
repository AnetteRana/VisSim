#include "graph3d.h"
#include <cmath>

Graph3d::Graph3d(int sections, GLfloat width)
{
    float halfWidth = width/2;
    float strideLength = width/sections;
    int numberOfSquares = sections*sections;
    int numberOfTriangles = numberOfSquares*2;


    mSections = sections;
    mWidth = width;

    GLfloat size = 2*width/(sections);

    Vector3d up{0.f, 1.f, 0.f};

    GLfloat currentX;
    GLfloat currentY;

    // make vertices
    for (int i = 0; i <= sections; i++)
    {
        for (int j = 0; j <= sections; j++)
        {
            mVertices.push_back(Vertex{{-halfWidth+j*strideLength, 0, -halfWidth+i*strideLength}, up});
            mVertices.back().mPosition.y = getFunctionHeight(mVertices.back().mPosition.x, mVertices.back().mPosition.z);
            //mVertices.back().setUV( i*1.f/sections, j*1.f/sections ); // stretch texture
            mVertices.back().setUV( i,j );
        }
    }

    int trianglesPerRow = sections*2;
    int rowID = 0;
    // make indices
    for (int i = 0; i < numberOfTriangles; i++)
    {
        int rowID = i/trianglesPerRow;

        if(i == 0 || i%2 == 0)
        {
            mIndices.push_back((i/2)+rowID); // down left

            mIndices.push_back((i/2)+rowID+2+sections); // top right
            mIndices.push_back((i/2)+rowID+1); // down right
        }
        else
        {
            mIndices.push_back((i/2)+rowID); // down left

            mIndices.push_back((i/2)+rowID+1+sections); // top left
            mIndices.push_back((i/2)+rowID+2+sections); // top right
        }
    }
}

void Graph3d::init(GLint matrixUniform)
{
    calculateNormals();
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // element buffer object
    glGenBuffers(1, &mEBO);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mEBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof( GLuint ), &mIndices[0], GL_STATIC_DRAW );

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

void Graph3d::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{
    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }

    //makeMesh();
    //mMatrix.rotate(.5f, 0, 0, 1);

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    //glDrawArrays(GL_TRIANGLES, 0, signed(mVertices.size()));
    glDrawElements(GL_TRIANGLES, mIndices.size(),GL_UNSIGNED_INT, nullptr);
}

GLfloat Graph3d::getNormalX(GLfloat x)
{
    return -((3*sin(3*x))/5);
}

GLfloat Graph3d::getFunctionHeight(GLfloat x, GLfloat y)
{
    //return (cos(x*5)+cos(y*5))*0.2f;
    //return (cos(x*3)+cos(y*3))*0.2f;
    //GLfloat ran = ((rand()%1000)/1000.f)*5-2.5f;
    //GLfloat ran = sin(x)*sin(y)+((rand()%1000)/1000.f)*5-2.5f;
    //    GLint maxHeight = 20;
    //    GLint tilesPerLand = 55;
    //    GLint height = rand()%(maxHeight+tilesPerLand);

    //    if (height < tilesPerLand)
    //        height = -1;
    //    else
    //    {
    //        height = height / tilesPerLand;
    //    }


    GLint waterPerLand = 5;
    GLint maxHeight = 20;

    GLint height{-1}; // assume it's water
    GLint isLandChecker = rand()%waterPerLand;

    if (isLandChecker == 0)
    {
        height = rand()%maxHeight;
        return GLfloat(height)/5.f;
    }
    if(height==0)
        height =-1;

    return height;
}

void Graph3d::calculateNormals()
{
    //    int vertsInRow = mSections+1;

    //    for (int i = 0; i < mVertices.size(); i++)
    //    {
    //        // once per vert
    //        Vector3d u = mVertices[i+1].mPosition - mVertices[i].mPosition;
    //        Vector3d v = mVertices[i+2].mPosition - mVertices[i].mPosition;
    //        Vector3d currentNormal = u^v;

    //        qDebug() << "calculated normals in graph3d";
    //    }

    //    for (int i = 0; i < mVertices.size(); i+=3)
    //    {
    //        Vector3d u = mVertices[i+1].mPosition - mVertices[i].mPosition;
    //        Vector3d v = mVertices[i+2].mPosition - mVertices[i].mPosition;
    //        Vector3d currentNormal = u^v;
    //        mVertices[i].mColor = currentNormal;
    //        mVertices[i+1].mColor = currentNormal;
    //        mVertices[i+2].mColor = currentNormal;
    //        qDebug() << "calculated normals in graph3d";
    //    }
}
