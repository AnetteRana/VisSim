#include "recursivetriangles.h"

RecursiveTriangles::RecursiveTriangles(GLfloat radius, GLint recursions)
{
    int points = 3; // must be, so don't make this a parameter

    Vector3d color{0.76f, 0.69f, 0.1f};

    Matrix4x4 currentPlaceMat;
    currentPlaceMat[{0,0}] = 0;
    currentPlaceMat[{1,1}] = radius;
    currentPlaceMat[{2,2}] = 0;

    GLfloat degToRad = 3.14/180;
    GLfloat s = sin(degToRad* (360/points));
    GLfloat c = cos(degToRad* (360/points));

    // make outer points
    for (int p = 0; p < points; p++)
    {
        mVertices.push_back(Vertex{Vector3d{currentPlaceMat[{0,0}], currentPlaceMat[{1,1}], currentPlaceMat[{2,2}]}, color});

        GLfloat xnew =  currentPlaceMat[{0,0}] * c -  currentPlaceMat[{1,1}] *s;
        GLfloat ynew =  currentPlaceMat[{0,0}] * s +  currentPlaceMat[{1,1}] *c;
        currentPlaceMat[{0,0}] = xnew;
        currentPlaceMat[{1,1}] = ynew;

        //currentPlaceMat.rotate(360/points, Vector3d(0,0,1));
    }

    std::vector<Vertex> temp;

    for (int r = 0; r < recursions; r++)
    {
        for (int k = 0; k < mVertices.size(); k+=3) // for each triangle
        {

            // for each 3rd
            for (int i = k; i < k+3; i++)
            {
                //first pop self to temp
                temp.push_back(Vertex{mVertices[i].mPosition, color});

                // if i = 0
                if (i == k)
                {
                    Vector3d position = mVertices[i].mPosition +(mVertices[i+1].mPosition - mVertices[i].mPosition)*0.5f;
                    temp.push_back(Vertex{position, color});
                    position = mVertices[i].mPosition +(mVertices[i+2].mPosition - mVertices[i].mPosition)*0.5f;
                    temp.push_back(Vertex{position, color});
                }
                else if (i == k+1)
                {
                    Vector3d position = mVertices[i].mPosition +(mVertices[i+1].mPosition - mVertices[i].mPosition)*0.5f;
                    temp.push_back(Vertex{position, color});
                    position = mVertices[i].mPosition +(mVertices[i-1].mPosition - mVertices[i].mPosition)*0.5f;
                    temp.push_back(Vertex{position, color});
                }
                else if (i == k+2)
                {
                    Vector3d position = mVertices[i].mPosition +(mVertices[i-1].mPosition - mVertices[i].mPosition)*0.5f;
                    temp.push_back(Vertex{position, color});
                    position = mVertices[i].mPosition +(mVertices[i-2].mPosition - mVertices[i].mPosition)*0.5f;
                    temp.push_back(Vertex{position, color});
                }
            }

        }
        // fill mVertices from temp
        mVertices.clear();
        for (int i = 0; i < temp.size(); i++)
        {
            mVertices.push_back(temp[i]);
        }
        temp.clear();
    }
}

void RecursiveTriangles::init(GLint matrixUniform)
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

void RecursiveTriangles::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{
    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }

    //myRuntime += 1;

    //mMatrix.rotate(sin(myRuntime/180), Vector3d{1,0,0});
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, signed(mVertices.size()));
}
