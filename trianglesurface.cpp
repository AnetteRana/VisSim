#include "trianglesurface.h"
#include "collisionsphere.h"


#include "interactiveobject.h"

TriangleSurface::TriangleSurface() : VisualObject() {
    Vertex v{};
    v.setPosition(0,0,0);     v.setNormals(1,0,0); mVertices.push_back(v);
    v.setPosition(0.5,0,0);   v.setNormals(0,1,0); mVertices.push_back(v);
    v.setPosition(0.5,0.5,0); v.setNormals(0,0,1); mVertices.push_back(v);
    v.setPosition(0,0,0);     v.setNormals(0,1,0); mVertices.push_back(v);
    v.setPosition(0.5,0.5,0); v.setNormals(1,0,0); mVertices.push_back(v);
    v.setPosition(0,0.5,0);   v.setNormals(0,0,1); mVertices.push_back(v);

    mMatrix.setToIdentity();
}

TriangleSurface::TriangleSurface(std::string filename) : VisualObject()
{
    readFile(filename);

    //mCollisionObject = new CollisionSphere(this, 1);
}

void TriangleSurface::construct()
{
    GLfloat mPI = 3.14;

    float xmin=0.0f, xmax=1.0f, ymin=0.0f, ymax=1.0f, h=0.25f;
    for (auto x=xmin; x<xmax; x+=h)
        for (auto y=ymin; y<ymax; y+=h)
        {
            float z = sin(mPI*x)*sin(mPI*y);
            mVertices.push_back(Vertex{Vector3d{x,y,z},Vector3d{x,y,z}});
            z = sin(mPI*(x+h))*sin(mPI*y);
            mVertices.push_back(Vertex{Vector3d{x+h,y,z},Vector3d{x,y,z}});
            z = sin(mPI*x)*sin(mPI*(y+h));
            mVertices.push_back(Vertex{Vector3d{x,y+4,z},Vector3d{x,y,z}});
            mVertices.push_back(Vertex{Vector3d{x,y+h,z},Vector3d{x,y,z}});
            z = sin(mPI*(x+h))*sin(mPI*y);
            mVertices.push_back(Vertex{Vector3d{x+h,y,z},Vector3d{x,y,z}});
            z = sin(mPI*(x+h))*sin(mPI*(y+h));
            mVertices.push_back(Vertex{Vector3d{x+h,y+h,z},Vector3d{x,y,z}});
        }
}


void TriangleSurface::readFile(std::string filename)
{
    float maxX{0}, minX{0}, maxY{0}, minY{0}, maxZ{0}, minZ{0}; // to log while reading data
    //float wanted_maxX{5.f},wanted_maxY{5.f}, wanted_maxZ{5.f};

    std::ifstream inn;
    inn.open(filename.c_str());

    if (inn.is_open())
    {
        int n;
        //Vertex vertex;
        Vector3d vector;
        inn >> n;
        mVertices.reserve(n);
        for (int i=0; i<n; i++) {
            //inn >> vertex.mPosition.x >> vertex.mPosition.y >> vertex.mPosition.z;
            inn >> vector.x >> vector.y >> vector.z;
            //mVertices.push_back(vertex);
            mDatapoints.push_back(vector);

            if (i==0) // making sure to use real data for max/min
            {
                maxX = vector.x; minX = vector.x;
                maxY = vector.y; minY = vector.y;
                maxZ = vector.z; minZ = vector.z;
            }

            // store max/min
            if (vector.x > maxX)
                maxX = vector.x;
            if (vector.x < minX)
                minX = vector.x;
            if (vector.y > maxY)
                maxY = vector.y;
            if (vector.y < minY)
                minY = vector.y;
            if (vector.z > maxZ)
                maxZ = vector.z;
            if (vector.z < minZ)
                minZ = vector.z;
        }
        inn.close();
    }

    /*
    // change positions to fit desired size (and place)
    float centerX = minX +((maxX-minX)/2);
    float centerY = minY +((maxY-minY)/2);
    float centerZ = minZ +((maxZ-minZ)/2);
    for (int i = 0; i < mVertices.size(); i++)
    {
        mVertices[i].mPosition.x = mVertices[i].mPosition.x - centerX;
        mVertices[i].mPosition.y = mVertices[i].mPosition.y - centerY;
        mVertices[i].mPosition.z = mVertices[i].mPosition.z - centerZ;
    }
    */

    //    // make indices
    //    for (int i = 0; i < mVertices.size(); i++)
    //    {
    //        mIndices.push_back(i);
    //    }





    makeGrid();
    setNeighbours();
    giveTrianglesNormals();
}

// makes the grid, but hight on 0 still
void TriangleSurface::makeGrid()
{
    float maxX = radius;
    float minX = -radius;
    float maxZ = radius;
    float minZ = -radius;
    int subdiv = radius*2;

    float spacingX = (maxX - minX)/subdiv; // (maxX - minX) = distanceX
    float spacingZ = (maxZ - minZ)/subdiv;

    for (int i = 0; i <= subdiv; i++)
    {
        for (int j = 0; j <= subdiv; j++)
        {
            mVertices.push_back(Vertex(Vector3d(minX+(i*spacingX), 0, minZ+(j*spacingZ))));
        }
    }

    // make indices
    // each loop is a square
    for (int i = 0; i <= (subdiv)*(subdiv)+(subdiv-2); i++)
    {
        if ((i+1)%(subdiv+1) != 0)
        {
            mIndices.push_back(i);
            mIndices.push_back(i+1);
            mIndices.push_back(i+subdiv+1);

            mIndices.push_back(i+1);
            mIndices.push_back(i+subdiv+2);
            mIndices.push_back(i+subdiv+1);
        }
    }

    //setNeighbours();

    //    // randomize heights
    //    for (int i = 0; i < mVertices.size(); i++)
    //    {
    //        mVertices[i].mPosition.setY(std::rand()%3);
    //    }

    // set height based on datapoints
    float vertexRadius = .5f;
    int points = 0;
    float height = 0;
    for (int i = 0; i < mVertices.size(); i++)
    {
        points = 0;
        height = 0;
        for (int j = 0; j < mDatapoints.size(); j++)
        {
            // if datapoint is within radius of vertex
            if ((mDatapoints[j].x < (mVertices[i].mPosition.x + vertexRadius))&&(mDatapoints[j].z < (mVertices[i].mPosition.z + vertexRadius)))
            {
                // add height
                points++;
                height += mDatapoints[j].y;
                // delete point
                mDatapoints.erase(mDatapoints.begin()+j);
                j--;
            }
        }
        if (points > 0)
            height = height/points;
        mVertices[i].mPosition.y = height;
    }

}

void TriangleSurface::setNeighbours()
{
    mNeighbour.reserve(mIndices.size());

    // loop for each index, to set its neighbour
    for (uint i = 0; i < mIndices.size(); i++)
    {
        // in case there is no neighbour
        mNeighbour.push_back(-1);

        // use barysentric to find a point outside the edge
        // first index = i (then i+1, i+2)
        // triangle ABC
        Vector2d A, B, C;

        if (i%3 == 0)
        {
            // A
            A = Vector2d(mVertices[mIndices[i]].mPosition.getX(), mVertices[mIndices[i]].mPosition.getZ());
            B = Vector2d(mVertices[mIndices[i+1]].mPosition.getX(), mVertices[mIndices[i+1]].mPosition.getZ());
            C = Vector2d(mVertices[mIndices[i+2]].mPosition.getX(), mVertices[mIndices[i+2]].mPosition.getZ());
        }
        else if ((i-1)%3 == 0)
        {
            // B
            A = Vector2d(mVertices[mIndices[i]].mPosition.getX(), mVertices[mIndices[i]].mPosition.getZ());
            B = Vector2d(mVertices[mIndices[i+1]].mPosition.getX(), mVertices[mIndices[i+1]].mPosition.getZ());
            C = Vector2d(mVertices[mIndices[i-1]].mPosition.getX(), mVertices[mIndices[i-1]].mPosition.getZ());
        }
        else
        {
            // C
            A = Vector2d(mVertices[mIndices[i]].mPosition.getX(), mVertices[mIndices[i]].mPosition.getZ());
            B = Vector2d(mVertices[mIndices[i-2]].mPosition.getX(), mVertices[mIndices[i-2]].mPosition.getZ());
            C = Vector2d(mVertices[mIndices[i-1]].mPosition.getX(), mVertices[mIndices[i-1]].mPosition.getZ());
        }

        // set testspot outside current triangle (so in the neighbour, opposite A)
        Vector2d testSpot;
        testSpot = testSpot.baryNeighbour(A,B,C);

        // loop through triangles until you find the triangle its in
        for (uint firstIndexOfTriangle = 0; firstIndexOfTriangle <= mIndices.size()-3; firstIndexOfTriangle+=3)
        {
            // triangle DEF
            Vector2d D(mVertices[mIndices[firstIndexOfTriangle]].mPosition.getX(), mVertices[mIndices[firstIndexOfTriangle]].mPosition.getZ());
            Vector2d E(mVertices[mIndices[firstIndexOfTriangle+1]].mPosition.getX(), mVertices[mIndices[firstIndexOfTriangle+1]].mPosition.getZ());
            Vector2d F(mVertices[mIndices[firstIndexOfTriangle+2]].mPosition.getX(), mVertices[mIndices[firstIndexOfTriangle+2]].mPosition.getZ());

            // get the barysentric coordinates of the spot
            Vector3d bary = testSpot.barycentricCoordinates(D,E,F);


            // if inside
            if (bary.x > 0.f && bary.y > 0.f && bary.z > 0.f)
            {
                // in the neighbour, set mNeighbour to this triangle
                mNeighbour[i] = firstIndexOfTriangle/3;
                //break;
            }
        }
    }
}

void TriangleSurface::giveTrianglesNormals()
{
    for (uint i = 0; i < mIndices.size(); i+=3)
    {
        // find triangle normal
        Vector3d u = mVertices[mIndices[i+1]].mPosition - mVertices[mIndices[i]].mPosition;
        Vector3d v = mVertices[mIndices[i+2]].mPosition - mVertices[mIndices[i]].mPosition;
        Vector3d currentNormal = u^v;

        // set that normal on the three indices
        mVertices[mIndices[i]].mColor = currentNormal;
        mVertices[mIndices[i+1]].mColor = currentNormal;
        mVertices[mIndices[i+2]].mColor = currentNormal;
    }
}

void TriangleSurface::init(GLint shader)
{
    mMatrixUniform = shader;

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




    // element buffer object
    glGenBuffers(1, &mEBO);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mEBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof( GLuint ), &mIndices[0], GL_STATIC_DRAW );
    //qDebug() << mIndices.size();

    glBindVertexArray(0);
}

void TriangleSurface::draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{

    //    // for player
    //    mVelocity = mVelocity/1.09f;
    //    mMatrix[{0,3}] = mMatrix[{0,3}] + mVelocity.x;
    //    mMatrix[{1,3}] = mMatrix[{1,3}] + mVelocity.y;
    //    mMatrix[{2,3}] = mMatrix[{2,3}] + mVelocity.z;


    if(mShader)
    {
        mShader->use(PMatrixUniform, VMatrixUniform, currentCamera, lightSource);
    }
    //mVertices[1].print();
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_TRUE, mMatrix.constData());

    glPointSize(10.f);
    //glDrawElements(GL_POINTS, mIndices.size(),GL_UNSIGNED_INT, nullptr);
    glDrawElements(GL_TRIANGLES, mIndices.size(),GL_UNSIGNED_INT, nullptr);
}

void TriangleSurface::interactiveMove(GLfloat x, GLfloat y, GLfloat z)
{
    qDebug() << mVelocity.x;
    Vector3d addedSpeed {x, y, z};
    //qDebug() << "InteractiveObject::move";
    //mMatrix.translate(x, y, z);
    if (mVelocity.length() <= .5f) // max speed
        mVelocity = mVelocity + addedSpeed;
}

int TriangleSurface::findTriangle(InteractiveObject* ball)
{
    int triangleID = 0;
    // start with first triangle
    triangleID = (findTriangleRecursive(ball, triangleID));

    return triangleID;
}

int TriangleSurface::findTriangleRecursive(InteractiveObject* ball, int triangleID)
{
    // calculate barysentric coordinate

    Vector3d ballPos = ball->position;//{ball->mMatrix[{0, 3}], ball->mMatrix[{1, 3}], ball->mMatrix[{2, 3}]};

    Vector3d p0 = {mVertices[mIndices[(triangleID*3)]].mPosition};
    Vector3d p1 = {mVertices[mIndices[(triangleID*3)+1]].mPosition};
    Vector3d p2 = {mVertices[mIndices[(triangleID*3)+2]].mPosition};

    Vector3d baryCoord = ballPos.barycentricCoordinates(p0, p1, p2);

    if ( (baryCoord.x >= 0) && (baryCoord.y >= 0) && (baryCoord.z >= 0) ) // inside
    {
        return triangleID;
    }
    else if ((baryCoord.x < baryCoord.y) && (baryCoord.x < baryCoord.z))
    {
        // check x's neighbour
        if (mNeighbour[(triangleID*3)] < 0)
            return -1;
        else
            return findTriangleRecursive(ball, mNeighbour[(triangleID*3)]);
    }
    else if ((baryCoord.y < baryCoord.z) && (baryCoord.y < baryCoord.x))
    {
        // check y's neighbour
        if (mNeighbour[(triangleID*3)+1] < 0)
            return -1;
        else
            return findTriangleRecursive(ball, mNeighbour[(triangleID*3)+1]);
    }
    else if ((baryCoord.z < baryCoord.x) && (baryCoord.z < baryCoord.y))
    {
        // check z's neighbour
        if (mNeighbour[(triangleID*3)+2] < 0)
            return -1;
        else
            return findTriangleRecursive(ball, mNeighbour[(triangleID*3)+2]);
    }

    return -1; // outside of plane
}

void TriangleSurface::giveSurfaceTriangleToBall(InteractiveObject* ball)
{
    Vector3d ballPos = ball->position;// {ball->mMatrix[{0, 3}], ball->mMatrix[{1, 3}], ball->mMatrix[{2, 3}]};

    // first find the triangle
    int triangleID = findTriangle(ball);

    if (triangleID < 0)
    {
        qDebug() << "Outide of bounds";
    }
    else
    {
        // find barycoords of ball
        // update ball - give it the points of the triangle it's on
        ball->v0 = {mVertices[mIndices[(triangleID*3)]].mPosition};
        ball->v1 = {mVertices[mIndices[(triangleID*3)+1]].mPosition};
        ball->v2 = {mVertices[mIndices[(triangleID*3)+2]].mPosition};
    }
}




