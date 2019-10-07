#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSurfaceFormat>
#include <QDebug>

#include "renderwindow.h"
//#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete ui;
}

void MainWindow::init()
{
    //resize(QDesktopWidget().availableGeometry(this).size() * 1); // fullscreen
    QWidget::showMaximized();

    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    format.setVersion(4, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types that OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);

    //Set the number of samples used for multisampling - (antialiasing)
    format.setSamples(4);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The renderer will need a depth buffer - (not requiered to set in glfw-tutorials)
    format.setDepthBufferSize(24);

    //Just prints out what OpenGL format we will get
    // - this can be deleted
    qDebug() << "Requesting surface format: " << format;

    //We have a format for the OpenGL window, so let's make it:
    mRenderWindow = new RenderWindow(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderWindow->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    //OpenGLLayout is made in the .ui-file!
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);

    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();
}

//Example of a slot called from the button on the top of the program.

//Example of a slot called from the button on the top of the program.
//void MainWindow::on_wireframeButton_clicked()
//{
//    mRenderWindow->toggleWireframe();
//    mRenderWindowContainer->setFocus();
//}

void MainWindow::on_actionwireframe_triggered()
{
    mRenderWindow->toggleWireframe();
    //mRenderWindowContainer->setFocus();
}

void MainWindow::on_actiontoggleFaceCulling_triggered()
{
    mRenderWindow->toggleCulling();
}

void MainWindow::on_reset_triggered()
{
    mRenderWindow->theball->reset();
}

void MainWindow::on_timescale_valueChanged(double arg1)
{
    mRenderWindow->theball->timeScale = arg1;
}

void MainWindow::on_toggleSimulate_triggered()
{
    mRenderWindow->toggleSimulation();
}

void MainWindow::on_bounciness_valueChanged(double arg1)
{
    mRenderWindow->theball->bounciness = arg1;
}
