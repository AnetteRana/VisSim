#include "beziercurve.h"
#include <cmath>

BezierCurve::BezierCurve(bool curve, bool control, GLfloat curveSmoothness)
{
    mCurveSmoothness = curveSmoothness;

    points[0] = Vector3d{0  ,1  ,-2  };
    points[1] = Vector3d{-1  ,3  ,0  };
    points[2] = Vector3d{1   ,3  ,0  };
    points[3] = Vector3d{2   ,1  ,2  };

    if (curve)
        drawCurve();
    if (control)
        drawControl();
}

BezierCurve::BezierCurve(Vector3d a, Vector3d b, Vector3d c, Vector3d d, bool curve, bool control, GLfloat curveSmoothness)
{
    mCurveSmoothness = curveSmoothness;

    points[0] = a;
    points[1] = b;
    points[2] = c;
    points[3] = d;

    if (curve)
        drawCurve();
    if (control)
        drawControl();
}

void BezierCurve::init(GLint matrixUniform)
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

void BezierCurve::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{
    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }

    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    //glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
}

void BezierCurve::drawCurve()
{
    Vector3d color{0,0,1};
    for (GLfloat i = 0; i <= 1; i+= (1/mCurveSmoothness))
    {
        mVertices.push_back(Vertex{evaluateBezier(3, i), color});
    }
}

void BezierCurve::drawControl()
{
    // for å tegne "kontrollpolygon"
    Vector3d color{1,0,0};
    mVertices.push_back(Vertex{points[3], color});
    mVertices.push_back(Vertex{points[2], color});
    mVertices.push_back(Vertex{points[1], color});
    mVertices.push_back(Vertex{points[0], color});
}

Vector3d BezierCurve::evaluateBezier(int degree, GLfloat t)
{
    Vector3d tempPoints[4];

    for (int i=0; i<4; i++)
    {
        tempPoints[i] = points[i];
    }

    for (int k=1; k<=degree; k++)
    {
        for (int i=0; i<degree-k+1; i++)
        {
            tempPoints[i] = (tempPoints[i] * (1-t)) + (tempPoints[i+1] * t);
        }
    }

    return tempPoints[0];
}













