#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTime>

class openGLWindow : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

public:
    // variables

    // functions
    openGLWindow(QWidget *parent);
    ~openGLWindow();
protected:
    void  initializeGL() override;
    void	paintGL() override;
    //void	resizeGL(int w, int h) override;

public:
    void reset_scene();


private:
    // mouse events

};

#endif // OPENGLWINDOW_H
