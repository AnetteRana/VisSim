#ifndef SHADER_H
#define SHADER_H


#include <QOpenGLFunctions_4_1_Core>
#include "matrix4x4.h"
class Camera;
class VisualObject;

//This class is pretty much a copy of the shader class at
//https://github.com/SonarSystems/Modern-OpenGL-Tutorials/blob/master/%5BLIGHTING%5D/%5B8%5D%20Basic%20Lighting/Shader.h
//which is based on stuff from http://learnopengl.com/ and http://open.gl/.

//must inherit from QOpenGLFunctions_4_1_Core, since we use that instead of glfw/glew/glad
class Shader : protected QOpenGLFunctions_4_1_Core
{
public:
    // Constructor generates the shader on the fly
    Shader( const GLchar *vertexPath, const GLchar *fragmentPath );

    // Use the current shader
    void use(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource);

    //Get program number for this shader
    GLuint getProgram() const;

    //virtual void transmitUniformData(Matrix4x4 *modelMatrix, class Material *material = nullptr);

    void setCurrentCamera(Camera *currentCamera);

private:
    GLuint program;

protected:
    //GLint program{0};
    GLint mMatrixUniform{-1};
    GLint vMatrixUniform{-1};
    GLint pMatrixUniform{-1};

    Camera *mCurrentCamera{nullptr};
};

#endif
