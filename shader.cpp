#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "camera.h"
#include "VisualObject.h"

//#include "GL/glew.h" - using QOpenGLFunctions instead

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{
    initializeOpenGLFunctions();    //must do this to get access to OpenGL functions in QOpenGLFunctions

    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // Open files and check for errors
    vShaderFile.open( vertexPath );
    if(!vShaderFile)
        std::cout << "ERROR SHADER FILE " << vertexPath << " NOT SUCCESFULLY READ" << std::endl;
    fShaderFile.open( fragmentPath );
    if(!fShaderFile)
        std::cout << "ERROR SHADER FILE " << fragmentPath << " NOT SUCCESFULLY READ" << std::endl;
    std::stringstream vShaderStream, fShaderStream;
    // Read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf( );
    fShaderStream << fShaderFile.rdbuf( );
    // close file handlers
    vShaderFile.close( );
    fShaderFile.close( );
    // Convert stream into string
    vertexCode = vShaderStream.str( );
    fragmentCode = fShaderStream.str( );

    const GLchar *vShaderCode = vertexCode.c_str( );
    const GLchar *fShaderCode = fragmentCode.c_str( );

    // 2. Compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex Shader
    vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex, 1, &vShaderCode, nullptr );
    glCompileShader( vertex );
    // Print compile errors if any
    glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( vertex, 512, nullptr, infoLog );
        std::cout << "ERROR SHADER VERTEX " << vertexPath << " COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment Shader
    fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment, 1, &fShaderCode, nullptr );
    glCompileShader( fragment );
    // Print compile errors if any
    glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
    if ( !success )
    {
        glGetShaderInfoLog( fragment, 512, nullptr, infoLog );
        std::cout << "ERROR SHADER FRAGMENT " << fragmentPath << " COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Shader Program
    this->program = glCreateProgram( );
    glAttachShader( this->program, vertex );
    glAttachShader( this->program, fragment );
    glLinkProgram( this->program );
    // Print linking errors if any
    glGetProgramiv( this->program, GL_LINK_STATUS, &success );
    if (!success)
    {
        glGetProgramInfoLog( this->program, 512, nullptr, infoLog );
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED " << vertexPath << infoLog << std::endl;
    }
    // Delete the shaders as they're linked into our program now and no longer needed
    glDeleteShader( vertex );
    glDeleteShader( fragment );
}

void Shader::use(GLint PMatrixUniform, GLint VMatrixUniform, Camera* currentCamera, VisualObject* lightSource)
{
    glUseProgram( this->program );


    // update shader uniforms
    GLint lightPosLocation = glGetUniformLocation(this->getProgram(), "lightPos");
    glUniform3f(lightPosLocation, lightSource->mMatrix[{0,3}], lightSource->mMatrix[{1,3}]+.5f, lightSource->mMatrix[{2,3}]);
    GLint lightColorLocation = glGetUniformLocation(this->getProgram(), "lightColor");
    glUniform3f(lightColorLocation, 1,.9f,0.8f);
    GLint ambientStrengthLocation = glGetUniformLocation(this->getProgram(), "ambientStrength");
    glUniform1f(ambientStrengthLocation, .6f);
    GLint ambientLightColorLocation = glGetUniformLocation(this->getProgram(), "ambientLightColor");
    glUniform3f(ambientLightColorLocation, 0.1f, 0.1f, 1);

    // other uniforms
    mMatrixUniform = glGetUniformLocation( this->getProgram(), "matrix" );
    PMatrixUniform = glGetUniformLocation( this->getProgram(), "pMatrix" );
    VMatrixUniform = glGetUniformLocation( this->getProgram(), "vMatrix" );
    glUniformMatrix4fv( PMatrixUniform, 1, GL_FALSE, currentCamera->mPMatrix->constData());
    glUniformMatrix4fv( VMatrixUniform, 1, GL_FALSE, currentCamera->mVMatrix->constData());
}

GLuint Shader::getProgram() const
{
    return program;
}
