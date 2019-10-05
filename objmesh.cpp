#include "objmesh.h"
#include <vector>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "collisionsphere.h"

ObjMesh::ObjMesh() : VisualObject ()
{

}

ObjMesh::ObjMesh(std::string objectName, std::string filename, Vector3d inTrans, Vector3d inRot, float rotAmount, Vector3d inScale) : VisualObject ()
{
    mName = objectName;

    readFile(filename);
    mMatrix.setToIdentity();

    //Transformasjon
    mMatrix.translate(inTrans);
    mMatrix.scale(inScale);
    mMatrix.rotate(rotAmount, inRot);

    mCollisionObject = new CollisionSphere(this, 1);
}

ObjMesh::~ObjMesh()
{
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

void ObjMesh::readFile(std::string filename)
{
    //Open File
    std::ifstream fileIn;
    fileIn.open (filename, std::ifstream::in);
    if(!fileIn)
        qDebug() << "Could not open file for reading: " << QString::fromStdString(filename);

    //One line at a time-variable
    std::string oneLine;
    //One word at a time-variable
    std::string oneWord;

    std::vector<Vector3d> tempVertecies;
    std::vector<Vector3d> tempNormals;
    std::vector<Vector2d> tempUVs;

    // Varible for constructing the indices vector
    unsigned int temp_index = 0;

    //Reading one line at a time from file to oneLine
    while(std::getline(fileIn, oneLine))
    {
        //Doing a trick to get one word at a time
        std::stringstream sStream;
        //Pushing line into stream
        sStream << oneLine;
        //Streaming one word out of line
        oneWord = ""; //resetting the value or else the last value might survive!
        sStream >> oneWord;

        if (oneWord == "#")
        {
            //Ignore this line
            //            qDebug() << "Line is comment "  << QString::fromStdString(oneWord);
            continue;
        }
        if (oneWord == "")
        {
            //Ignore this line
            //            qDebug() << "Line is blank ";
            continue;
        }
        if (oneWord == "v")
        {
            //            qDebug() << "Line is vertex "  << QString::fromStdString(oneWord) << " ";
            Vector3d tempVertex;
            sStream >> oneWord;
            tempVertex.x = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.y = std::stof(oneWord);
            sStream >> oneWord;
            tempVertex.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempVertecies.push_back(tempVertex);

            continue;
        }
        if (oneWord == "vt")
        {
            //            qDebug() << "Line is UV-coordinate "  << QString::fromStdString(oneWord) << " ";
            Vector2d tempUV;
            sStream >> oneWord;
            tempUV.x = std::stof(oneWord);
            sStream >> oneWord;
            tempUV.y = std::stof(oneWord);

            //UV made - pushing it into UV-vector
            tempUVs.push_back(tempUV);

            continue;
        }
        if (oneWord == "vn")
        {
            //            qDebug() << "Line is normal "  << QString::fromStdString(oneWord) << " ";
            Vector3d tempNormal;
            sStream >> oneWord;
            tempNormal.x = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.y = std::stof(oneWord);
            sStream >> oneWord;
            tempNormal.z = std::stof(oneWord);

            //Vertex made - pushing it into vertex-vector
            tempNormals.push_back(tempNormal);
            continue;
        }
        if (oneWord == "f")
        {
            //            qDebug() << "Line is a face "  << QString::fromStdString(oneWord) << " ";
            //int slash; //used to get the / from the v/t/n - format
            int index, normal, uv;
            for(int i = 0; i < 3; i++)
            {
                sStream >> oneWord;     //one word read
                std::stringstream tempWord(oneWord);    //to use getline on this one word
                std::string segment;    //the numbers in the f-line
                std::vector<std::string> segmentArray;  //temp array of the numbers
                while(std::getline(tempWord, segment, '/')) //splitting word in segments
                {
                    segmentArray.push_back(segment);
                }
                index = std::stoi(segmentArray[0]);     //first is vertex
                if (segmentArray[1] != "")              //second is uv
                    uv = std::stoi(segmentArray[1]);
                else
                {
                    //qDebug() << "No uvs in mesh";       //uv not present
                    uv = 0;                             //this will become -1 in a couple of lines
                }
                normal = std::stoi(segmentArray[2]);    //third is normal

                //Fixing the indexes
                //because obj f-lines starts with 1, not 0
                --index;
                --uv;
                --normal;

                if (uv > -1)    // If UV's are found
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], tempUVs[uv]);
                    mVertices.push_back(tempVert);
                }
                else            // If no UV's are found, use (0.f, 0.f) as UV's
                {
                    Vertex tempVert(tempVertecies[index], tempNormals[normal], Vector2d(0.f, 0.f));
                    mVertices.push_back(tempVert);
                }
                mIndices.push_back(temp_index++);
            }
        }
    }
    fileIn.close();
}

void ObjMesh::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void ObjMesh::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{
    //glBindTexture(GL_TEXTURE_2D, mTexture);

    // slow down speed
    if (mMoveSpeed > 2)
    {
        mMoveSpeed= mMoveSpeed/1.00001f;
    }
    if (mMoveSpeed < 2)
    {
        mMoveSpeed= 2;
    }

    // slow down rotation
    if (mRotSpeed > 2)
    {
        mRotSpeed= mRotSpeed/1.00001f;
    }
    if (mRotSpeed < 2)
    {
        mRotSpeed= 2;
    }


    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

// ========= MOVEMENT =========
void ObjMesh::move(float x, float y, float z, Vector3d surfNormal)
{
    mMatrix.translate(x, y, z);
    mx = mMatrix[{0,3}];
    my = mMatrix[{1,3}];
    mz = mMatrix[{2,3}];
}

void ObjMesh::rotate(float amount, int x, int y, int z)
{
    mMatrix.rotate(amount, Vector3d(x,y,z));
    mrx += x;
    mry += y;
    mrz += z;
}

void ObjMesh::setLocation(float inX, float inY, float inZ)
{
    mMatrix[{0,3}] = inX;
    mMatrix[{1,3}] = inY;
    mMatrix[{2,3}] = inZ;
}

Vector3d ObjMesh::getPosition()
{
    return Vector3d(mx, my, mz);
}

void ObjMesh::interactiveMove(GLfloat x, GLfloat y, GLfloat z)
{
    // forward
    if (z)
    {
        if (mMoveSpeed < 40)
        {
            mMoveSpeed = mMoveSpeed * 1.1f;
        }
        mMatrix.translate(0, 0, z*mMoveSpeed);
    }

    // turning
    if (x)
    {

        if (mRotSpeed < 40)
        {
            mRotSpeed = mRotSpeed * 1.02f;
        }
        mMatrix.rotate(-x*mRotSpeed, Vector3d(0,1,0));
        mRotation += Vector3d(0, -x*mRotSpeed, 0);
    }

}
