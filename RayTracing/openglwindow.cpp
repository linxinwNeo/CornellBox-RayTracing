#define GL_SILENCE_DEPRECATION
#include <QMouseEvent>
#include <QSurfaceFormat>
#include <QOpenGLShaderProgram>

#include "openglwindow.h"

double vor_MIN = 0, vor_MAX = 0;

// VAO saves data structures
// VBO saves actual data
// EBO saves indices info
unsigned int VAO, VBO,  EBO;
QOpenGLShaderProgram shaderProgram;

float vs[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

openGLWindow::openGLWindow(QWidget *parent) : QOpenGLWidget(parent)
{

    // set up format
    QSurfaceFormat fmt;
    fmt.setRenderableType(QSurfaceFormat::OpenGL);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setVersion( 3, 3 );
    fmt.setSamples(32);
    QSurfaceFormat::setDefaultFormat(fmt);
    this->setFormat(fmt);
}


openGLWindow::~openGLWindow()
{
    makeCurrent();
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    shaderProgram.removeAllShaders();
    doneCurrent();
}


void openGLWindow::initializeGL()
{
    this->initializeOpenGLFunctions();

}


void openGLWindow::paintGL()
{

}
