#include "plane.h"
#include "qdebug.h"

Plane::Plane(int sections, GLfloat width)
{
    if (sections > 0)
    {
        GLfloat size = 2*width/(sections);

        Vector3d colorA {0,1,0}; // normal straight up
        Vector3d colorB ( 0.565f, 0.998f, 0.467f);

        GLfloat currentX;
        GLfloat currentY;

        for(int i = 0; i < sections; i++)
        {
            for(int j = 0; j < sections; j++)
            {
                // low right
                currentX = (-width+(size*(0+i)));
                currentY = (-width+size*(0+j));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorA});
                currentX = (-width+(size*(1+i)));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorA});
                currentY = (-width+size*(1+j));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorA});

                // top left
                currentX = (-width+(size*(0+i)));
                currentY = (-width+size*(0+j));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorA});
                currentY = (-width+size*(1+j));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorA});
                currentX = (-width+(size*(1+i)));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorA});
            }
        }

        // hard code UV in the corners of 1x1 plane
        mVertices[0].setUV(0,0);
        mVertices[1].setUV(1,0);
        mVertices[2].setUV(1,1);
        mVertices[3].setUV(0,0);
        mVertices[4].setUV(0,1);
        mVertices[5].setUV(1,1);

        //        mVertices[0].setNormals(1,0,0);
        //        mVertices[1].setNormals(0,1,0);
        //        mVertices[2].setNormals(0,0,1);
        //        mVertices[3].setNormals(0,0,0);

    }
}

Plane::Plane(int sections, GLfloat width, GLfloat height)
{
    if (sections > 0)
    {
        GLfloat distanceWidth = 2*width/(sections);
        GLfloat distanceHeight = 2*height/(sections);

        Vector3d colorA {0,1,0};
        Vector3d colorB = colorA;

        GLfloat currentX;
        GLfloat currentY;

        for(int i = 0; i < sections; i++)
        {
            for(int j = 0; j < sections; j++)
            {
                // low right
                currentX = (-width+(distanceWidth*(0+i)));
                currentY = (-height+distanceHeight*(0+j));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorB});
                currentX = (-width+(distanceWidth*(1+i)));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorA});
                currentY = (-height+distanceHeight*(1+j));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorA});

                // top left
                currentX = (-width+(distanceWidth*(0+i)));
                currentY = (-height+distanceHeight*(0+j));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorB});
                currentY = (-height+distanceHeight*(1+j));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorA});
                currentX = (-width+(distanceWidth*(1+i)));
                mVertices.push_back(Vertex{{currentX, currentY, 0}, colorA});
            }
        }
    }
}

void Plane::init(GLint matrixUniform)
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



    // set texture ///////
    // glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(textureFileName.data(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);



    glBindVertexArray(0);
}

void Plane::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{

    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }


    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, signed(mVertices.size()));
}


