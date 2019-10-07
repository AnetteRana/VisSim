#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionwireframe_triggered();
    void on_actiontoggleFaceCulling_triggered();

    void on_reset_triggered();
    void on_timescale_valueChanged(double arg1);
    void on_toggleSimulate_triggered();

    void on_bounciness_valueChanged(double arg1);

private:
    void init();
    Ui::MainWindow *ui;

    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;
};

#endif // MAINWINDOW_H
