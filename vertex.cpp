#include "vertex.h"
#include <iostream>

Vertex::Vertex()
{

}

Vertex::Vertex(Vector3d position, Vector3d color)
{
    mPosition = position;
    mColor = color;
}

Vertex::Vertex(Vector3d position)
{
    mPosition = position;
}

Vertex::Vertex(Vector3d position, Vector3d color, Vector2d UV)
{
    mPosition = position;
    mColor = color;
    mTextureCoordinates = UV;
}

void Vertex::print()
{
        //qDebug() << "position: ";
    mPosition.print();
}

void Vertex::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
    mPosition = Vector3d(x, y, z);
}

void Vertex::setNormals(GLfloat x, GLfloat y, GLfloat z)
{
    mColor = Vector3d(x, y, z);
}

void Vertex::setUV(GLfloat u, GLfloat v)
{
    mTextureCoordinates = Vector2d(u, v);
}

std::ostream& operator<< (std::ostream& os, const Vertex& v)
{
    os << std::fixed;
    os << "(" << v.mPosition.getX() << ", " << v.mPosition.getY() << ", " << v.mPosition.getZ() << ") ";
    os << "(" << v.mColor.getX() << ", " << v.mColor.getY() << ", " << v.mColor.getZ() << ") ";
    os << "(" << v.mTextureCoordinates.getX() << ", " << v.mTextureCoordinates.getY() << ") ";
    return os;
}

std::istream& operator>> (std::istream& is, Vertex& v)
{
// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
  char dum, dum2, dum3, dum4;
  GLfloat x, y, z;
  is >> dum >> x >> dum2 >> y >> dum3 >> z >> dum4;
  v.mPosition.setX(x);
  v.mPosition.setY(y);
  v.mPosition.setZ(z);
  is >> dum >> x >> dum2 >> y >> dum3 >> z >> dum4;
  v.mColor.setX(x);
  v.mColor.setY(y);
  v.mColor.setZ(z);
  is >> dum >> x >> dum2 >> y >> dum3;
  v.mTextureCoordinates.setX(x);
  v.mTextureCoordinates.setY(y);
  return is;
}
