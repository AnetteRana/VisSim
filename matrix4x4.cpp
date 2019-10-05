#include "matrix4x4.h"
#include "qdebug.h"
#include <cmath>

Matrix4x4::Matrix4x4()
{
    setToIdentity();
}

Matrix4x4::Matrix4x4(Vector4d a, Vector4d b, Vector4d c, Vector4d d)
{
    for (int i = 0; i<4; i++)
    {
        data[0][i] = a.get(i);
        data[1][i] = b.get(i);
        data[2][i] = c.get(i);
        data[3][i] = d.get(i);
    }
}

Matrix4x4::Matrix4x4(GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
                     GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
                     GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
                     GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33 )
{
    data[0][0] = m00;
    data[1][0] = m10;
    data[2][0] = m20;
    data[3][0] = m30;

    data[0][1] = m01;
    data[1][1] = m11;
    data[2][1] = m21;
    data[3][1] = m31;

    data[0][2] = m02;
    data[1][2] = m12;
    data[2][2] = m22;
    data[3][2] = m32;

    data[0][3] = m03;
    data[1][3] = m13;
    data[2][3] = m23;
    data[3][3] = m33;
}

void Matrix4x4::print()
{
    for (int i = 0 ; i < 4; i++)
    {
        qDebug() << data[i][0] << " "  << data[i][1] << " "  << data[i][2] << " "  << data[i][3];
    }
}

GLfloat& Matrix4x4::operator[](const std::pair<int,int>& Index)
{
    return data[Index.first][Index.second];
}

GLfloat Matrix4x4::determinant()
{
    GLfloat a, b, c, d{0};

    for (int i = 0; i<3; i++)
    {
        detArray[0][i] = data[1][i+1];
        detArray[1][i] = data[2][i+1];
        detArray[2][i] = data[3][i+1];
    }
    a = ( data[0][0] * determinantHelper() );

    detArray[0][0] = data[1][0];
    detArray[1][0] = data[2][0];
    detArray[2][0] = data[3][0];
    b = ( data[0][1] * determinantHelper() );

    detArray[0][1] = data[1][1];
    detArray[1][1] = data[2][1];
    detArray[2][1] = data[3][1];
    c = ( data[0][2] * determinantHelper() );

    detArray[0][2] = data[1][2];
    detArray[1][2] = data[2][2];
    detArray[2][2] = data[3][2];
    d = ( data[0][3] * determinantHelper() );

    qDebug() <<  "abcd: "  << a << b << c << d;

    return (a-b+c-d);
}

GLfloat Matrix4x4::determinantHelper()
{
    GLfloat a, b, c{0};

    a = detArray[0][0] * ((detArray[1][1]*detArray[2][2]) - (detArray[1][2]*detArray[2][1]));
    b = detArray[0][1] * ((detArray[1][0]*detArray[2][2]) - (detArray[1][2]*detArray[2][0]));
    c = detArray[0][2] * ((detArray[1][0]*detArray[2][1]) - (detArray[1][1]*detArray[2][0]));
    return (a-b+c);
}

void Matrix4x4::setToIdentity()
{
    for (int i = 0 ; i < 4; i++)
    {
        for (int j = 0 ; j < 4; j++)
        {
            if (i==j)
                data[i][j] = 1;
            else
                data[i][j] = 0;
        }
    }
}

void Matrix4x4::setToZero()
{
    for (int i = 0 ; i < 4; i++)
    {
        for (int j = 0 ; j < 4; j++)
        {
            data[i][j] = 0;
        }
    }
}

void Matrix4x4::LU()
{
    int m{4};
    int n{4};
    for (int k=0; k<m-1; k++)
    {
        //pivot(k);
        // Ved radoperasjoner skal vi oppnå 0-ere under diagonalelementet
        // i alle rader nedenfor (altså kolonne k
        // Vi subtraherer et multiplum av k-te
        // rad fra radene nedenfor, nuller ut kolonner fra venstre
        for (int i=k+1; i<m; i++)
        {
            // Skal mult med denne og deretter trekke fra rad i
            // denne skal bli null og vi kan lagre faktoren her
            //if (data[k][k] != 0) // avoid devision by 0
            data[i][k] = data[i][k] / data[k][k];

            for (int j=k+1; j<n; j++)
            {
                // kolonnene til høyre for den som blir nullet ut
                data[i][j] = data[i][j] - data[i][k]*data[k][j];
            }
        }
    }
}

Vector4d Matrix4x4::solve(Vector4d &b) const {
    int m{4};
    int n{4};
    Vector4d x;

    for (int k=0; k<m; k++)
        for (int i=k+1; i<n; i++)
            b[i] = b[i]-data[i][k]*b[k];

    for (int i=n-1; i>=0; i--) {
        x[i] = b[i];
        for (int j=i+1; j<n; j++)
            x[i] = x[i] - data[i][j]*x[j];
        x[i] = x[i]/data[i][i];
    }
    return x;
}

Matrix4x4 Matrix4x4::projectionMatrix() const
{
    return Matrix4x4(); // fix this later
}

Matrix4x4 Matrix4x4::inverseMatrix() const
{
    return Matrix4x4(); // fix this later
}

Matrix4x4 Matrix4x4::transposed() const
{
    Matrix4x4 transp;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
                transp[{i,j}] = data[j][i];
        }
    }

    return transp;
}

Matrix4x4 Matrix4x4::operator * (const GLfloat& c) const
{    
    Matrix4x4 tempMat;

    for (int m = 0; m < 4; m++)
    {
        for (int n = 0; n < 4; n++)
        {
            tempMat[{m, n}] = data[m][n] * c;
        }
    }
    return tempMat;
}

Vector4d Matrix4x4::operator * (Vector4d& v) const
{
    Vector4d tempVec;

    for (int m = 0; m < 4; m++)
    {
        for (int n = 0; n < 4; n++)
        {
            tempVec[m] += data[m][n] * v[n];
        }
    }

    return tempVec;
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4& mat) const
{
    Matrix4x4 tempMatrix;
    tempMatrix.setToZero();

    for (int m = 0; m < 4; m++)
    {
        for (int n = 0; n < 4; n++)
        {
            for (int i = 0; i < 4; i++)
            {
                tempMatrix.data[m][n] += ( (data[m][i] * mat.data[i][n] ));
            }
        }
    }
    return tempMatrix;
}

void Matrix4x4::scale(GLfloat value)
{
    Matrix4x4 temp;
    temp[{0,0}] = value;
    temp[{1,1}] = value;
    temp[{2,2}] = value;

    *this = (*this * temp);
}

void Matrix4x4::scale(GLfloat x, GLfloat y, GLfloat z)
{
    Matrix4x4 temp;
    temp[{0,0}] = x;
    temp[{1,1}] = y;
    temp[{2,2}] = z;

    *this = (*this * temp);
}

void Matrix4x4::scale(Vector3d& v)
{
    Matrix4x4 temp;
    temp[{0,0}] = v.getX();
    temp[{1,1}] = v.getY();
    temp[{2,2}] = v.getZ();

    *this = (*this * temp);
}

void Matrix4x4::rotate(GLfloat angle, Vector3d v)
{
    angle = (angle/180)* 3.14159265358979323846;
    Matrix4x4 temp;

    if ( (v.getX()+v.getY()+v.getZ()) != 1 )
    {
        // don't rotate if vector is in wrong form
    }
    else if (v.getX() == 1)
    {
        temp.data[1][1] = cos(angle);
        temp.data[1][2] = -sin(angle);
        temp.data[2][1] = sin(angle);
        temp.data[2][2] = cos(angle);
    }
    else if (v.getY() == 1)
    {
        temp.data[0][0] = cos(angle);
        temp.data[2][0] = -sin(angle);
        temp.data[0][2] = sin(angle);
        temp.data[2][2] = cos(angle);
    }
    else if (v.getZ() == 1)
    {
        temp.data[0][0] = cos(angle);
        temp.data[0][1] = -sin(angle);
        temp.data[1][0] = sin(angle);
        temp.data[1][1] = cos(angle);
    }

    *this = (*this * temp);
}

void Matrix4x4::translate(GLfloat x, GLfloat y, GLfloat z)
{
    Matrix4x4 temp;
    temp[{0,3}] = x;
    temp[{1,3}] = y;
    temp[{2,3}] = z;

    *this = (*this * temp);
}

void Matrix4x4::translate(Vector3d& v)
{
    Matrix4x4 temp;
    temp[{0,3}] = v.getX();
    temp[{1,3}] = v.getY();
    temp[{2,3}] = v.getZ();

    *this = (*this * temp);
}

const GLfloat* Matrix4x4::constData()
{
    return *data;
}

// Transformasjon fra xyz til uvn, se Angel(7th ed) 5.3.2
// up - orienteringen av kamera, trenger ikke ligge i viewplan
// u = v x n gir riktig retning i høyrehåndssystem.
// n er normal på viewplan, retning mot kamera (som z-aksen er default)
void Matrix4x4::lookAt(const Vector3d &eye, const Vector3d &at, const Vector3d &up)
{
    /*
   Matrix4x4 M;
   M.setToIdentity();
   Vector3d v = up;
   v.normalize();          // y-akse i kameraframe
   Vector3d n = eye-at;
   n.normalize();          // z-akse i kameraframe
   Vector3d u = v.cross(n);
   u.normalize();          // x-akse i kameraframe
   v = n.cross(u);
   v.normalize();
   M(0,0) = u.x;  M(0,1) = u.y;  M(0,2) = u.z;
   M(1,0) = v.x;  M(1,1) = v.y;  M(1,2) = v.z;
   M(2,0) = n.x;  M(2,1) = n.y;  M(2,2) = n.z;
   M.translate(-eye.x, -eye.y, -eye.z);
   mult(M);		// *this * M
   */
}

// Se Angel kapittel 5.7.2
void Matrix4x4::frustum(float left, float right, float bottom, float top, float near, float far)
{
    /*
   // Forutsetter a_ij initielt 0
   A[0][0] = 2*near/(right-left);
   A[1][1] = 2*near/(top-bottom);
   A[0][2] = (right+left)/(right-left);
   A[1][2] = (top+bottom)/(top-bottom);
   A[2][2] = -(far+near)/(far-near);
   A[2][3] = -2*far*near/(far-near);
   A[3][2] = -1.0f;
   A[3][3] = 0.0f;
   */
}



