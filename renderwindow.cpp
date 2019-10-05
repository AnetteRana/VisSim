#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>

#include "shader.h"
#include "mainwindow.h"

#include "VisualObject.h"
#include "axis.h"
#include "graph2d.h"
#include "graph3d.h"
#include "plane.h"
#include "tetrahedron.h"
#include "cube.h"
#include "star.h"
#include "trianglesurface.h"

#include "matrix4x4.h"
#include "vector3d.h"
#include "vertex.h"
#include "collision.h"
#include "collisionboxaabb.h"
#include "collisionsphere.h"
#include "recursivetriangles.h"
#include <cmath>

#include "stb_image.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //    //This is the matrix used to transform the triangle
    //    //You could do without, but then you have to simplify the shader and shader setup
    //    mMVPmatrix = new QMatrix4x4{};
    //    mMVPmatrix->setToIdentity();

    mMMatrix = new QMatrix4x4{};
    mVMatrix = new QMatrix4x4{};
    mPMatrix = new QMatrix4x4{};

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}


/// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Connect the gameloop timer to the render function:
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        qDebug() << "makeCurrent() failed";
        return;
    }

    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;

    //must call this to use OpenGL functions
    initializeOpenGLFunctions();

    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();

    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);    //enables depth sorting - must use GL_DEPTH_BUFFER_BIT in glClear
    //glEnable(GL_CULL_FACE);
    glClearColor(0.2f, 0.3f, 0.5f,1.0f);    //color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
    mShaderProgram = new Shader("../VisSim/plainvertex.vert", "../VisSim/plainfragment.frag");
    mShaderProgramTexture = new Shader("../VisSim/texturevertex.vert", "../VisSim/texturefragment.frag");
    mShaderProgramPhong = new Shader("../VisSim/phong.vert", "../VisSim/phong.frag");
    mShaderProgramHeight = new Shader("../VisSim/height.vert", "../VisSim/height.frag");
    //*************************************************************************************************************************************************


    //*************************************************************************************************************************************************
    //*****************************************
    //*********************** Make objects here
    //*****************************************

    //*******************************
    //*********************** Scene 3
    //*******************************

    // light?
    mObjectsScene3.push_back(new InteractiveObject(1));
    mObjectsScene3.back()->mMatrix.translate(0, 4, 0);
    mObjectsScene3.back()->mMatrix.scale(.1f);
    mLightSource = mObjectsScene3.back();

    // axis
    mObjectsScene3.push_back(new Axis());
    mObjectsScene3.back()->mShader = mShaderProgram;

    // trekantbakke
    newGround = (new TriangleSurface(std::string("../VisSim/meshes/testBakke.txt")));
    newGround->mShader = mShaderProgramHeight;
    mObjectsScene3.push_back(newGround);

    // ball
    theball = (new InteractiveObject(1));
    theball->mMatrix.translate(2.9f, 0, 2.1f); // start pos
    theball->startPos = Vector3d(2.9f, 0, 2.1f);
    theball->mMatrix.scale(.1f);
    theball->mShader = mShaderProgramPhong;
    mObjectsScene3.push_back(theball);
    theball->mGround = newGround;

    //*****************************************************************************************************************************
    //*******************************
    //*********************** Cameras
    //*******************************

    // NB: enable in shader and in render() function also to use matrix
    //mMatrixUniform = glGetUniformLocation( mShaderProgramTexture->getProgram(), "matrix" );
    mMatrixUniform = glGetUniformLocation( mShaderProgramPhong->getProgram(), "matrix" );
    //mMatrixUniform = glGetUniformLocation( mShaderProgramPhong->getProgram(), "lightSource" );

    mCamera.push_back(new Camera{Vector3d(0, 6, 6), true});
    mCamera.push_back(new Camera{Vector3d(6, 5, 6), true});

    mCamera[0]->setLookAtTarget(new Plane);
    mCamera[1]->setLookAtTarget(new Plane); // lazy way to look at center

    mCamera[0]->init(mShaderProgramPhong);
    mCamera[1]->init(mShaderProgramPhong);

    for (auto it = mObjectsScene3.begin(); it != mObjectsScene3.end(); it++)
    {
        (*it)->init(mMatrixUniform);
    }

    glBindVertexArray( 0 );

}

///Called each frame - doing the rendering
void RenderWindow::render()
{

    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    if (isSimulating)
    {
    // update the ball
    //newGround->setBallHeight(theball);
    theball->move();
    }

    //to clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCamera[currentCamera]->perspective(mAspectRatio);
    mCamera[currentCamera]->update();

    if (currentCamera == 1)
    {
        GLfloat slowdown{15};
        mCamera[1]->mPosition.setX(10*cos(myRuntime/slowdown));
        mCamera[1]->mPosition.setZ(10*sin(myRuntime/slowdown));
    }

    //*************************************************************************************************************************
    //*********************** Drawing

    for (auto it = mObjectsScene3.begin(); it != mObjectsScene3.end(); it++)
    {
        (*it)->draw(mPMatrixUniform, mVMatrixUniform, mCamera[currentCamera], mLightSource);
    }

    //Calculate framerate
    calculateFramerate();
    mContext->swapBuffers(this);

    myRuntime+=.1f;
}

//This function is called from Qt when window is exposed (shown)
//and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    GLfloat w = width() * retinaScale;
    GLfloat h = height() * retinaScale;
    mAspectRatio = w/h;

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact..)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

//The way this is set up is that we start the clock before doing the draw call,
//and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)    //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30) //once pr 30 frames = update the message twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 60 frames
        }
    }
}

/// Uses QOpenGLDebugLogger if this is present
/// Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
            qDebug() << message;
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            qDebug() << "glGetError returns " << err;
        }
    }
}

/// Tries to start the extended OpenGL debugger that comes with Qt
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            qDebug() << "This system can not use QOpenGLDebugLogger, so we revert to glGetError()";

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            qDebug() << "System can log OpenGL errors!";
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
                qDebug() << "Started OpenGL debug logger!";
        }
    }
}

void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape){ mMainWindow->close(); }

    // change camera
    if(event->key() == Qt::Key_1){ currentCamera = 0; }
    if(event->key() == Qt::Key_2){ currentCamera = 1; }
}

void RenderWindow::toggleWireframe()
{
    if (wireframeOn)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        wireframeOn=false;
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        wireframeOn=true;
    }
}

void RenderWindow::toggleCulling()
{
    if (cullingOn)
    {
        cullingOn= false;
        glDisable(GL_CULL_FACE);
    }
    else
    {
        cullingOn=true;
        glEnable(GL_CULL_FACE);
    }
}


