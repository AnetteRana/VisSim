#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>

#include <vector>
#include "VisualObject.h"
#include "interactiveobject.h"
#include "vertex.h"
#include "vector3d.h"
#include "axis.h"
#include "camera.h"
#include "octahedronball.h"
#include "trianglesurface.h"
#include "collision.h"
#include "collisionboxaabb.h"
#include "beziercurve.h"
#include "rmath.h"
#include "objmesh.h"

class QOpenGLContext;
class Shader;
class MainWindow;

/// This inherits from QWindow to get access to the Qt functionality and
/// OpenGL surface.
/// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
/// This is the same as using glad and glw from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:

    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;

//    void error(const QString &msg);
    TriangleSurface* door;
    bool doorOpen{false};

    void toggleWireframe();

    void toggleCulling();

    bool isSimulating{false};

private slots:
    void render();

private:
    void updateFirstPersonCam();
    void init();
    GLfloat myRuntime{0};
    std::vector<Camera*> mCamera{};
    uint currentCamera{0};

    // vector to hold pointers to all the objects
    std::vector<VisualObject*> mObjectsScene3;

    VisualObject* mLightSource{nullptr};

    // VisSim
    InteractiveObject* theball{nullptr};
    TriangleSurface* newGround{nullptr};

    QOpenGLContext *mContext;
    bool mInitialized;

    Shader *mShaderProgram{nullptr};
    Shader *mShaderProgramTexture{nullptr};
    Shader *mShaderProgramPhong{nullptr};
    Shader *mShaderProgramHeight{nullptr};
    unsigned int mTexture;

    GLint  mMatrixUniform;
    GLint  mPMatrixUniform;
    GLint  mVMatrixUniform;

    GLuint mVAO;
    GLuint mVBO;

    //QMatrix4x4 *mMVPmatrix; //The matrix with the transform for the object we draw
    QMatrix4x4 *mMMatrix; // model
    QMatrix4x4 *mVMatrix; // view
    QMatrix4x4 *mPMatrix; // projeksjonsmatrise

    QTimer *mRenderTimer;     //timer that drives the gameloop
    QElapsedTimer mTimeStart;       //time variable that reads the actual FPS

    MainWindow *mMainWindow;    //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};

    void checkForGLerrors();
    void calculateFramerate();
    void startOpenGLDebugger();

    bool wireframeOn{false};
    bool cullingOn{false};
    GLfloat mAspectRatio{0};

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // RENDERWINDOW_H
