#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H


#include <QOpenGLFunctions_4_1_Core>
#include <vector>
#include "VisualObject.h"
#include "vector3d.h"
#include "vertex.h"

class InteractiveObject;


//#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

class TriangleSurface : public VisualObject
{
private:
    void giveTrianglesNormals();
public:
    TriangleSurface();
    TriangleSurface(std::string filename);

    void construct();

    void readFile(std::string filename);

    void init(GLint shader) override;
    void draw(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource) override;

    void interactiveMove(GLfloat x, GLfloat y, GLfloat z);

    // VisSim
    std::vector<GLint> mNeighbour;
    std::vector<Vector3d> mDatapoints;

    void makeGrid();
    void setNeighbours();
    int findTriangle(InteractiveObject* ball);
    int findTriangleRecursive(InteractiveObject* ball, int triangleID);
    void setBallHeight(InteractiveObject* ball);
};

#endif // TRIANGLESURFACE_H
