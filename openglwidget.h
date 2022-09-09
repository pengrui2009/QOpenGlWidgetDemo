#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QTimer>
#include <QTime>
#include <QtMath>
class OpenGLWidget : public QOpenGLWidget,public QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();
protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w,int h) override;
    virtual void paintGL() override;
    virtual bool event(QEvent *e) override;
private:
    QVector<float> vertices;
    QVector<QVector3D> cubePositions;
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer VBO;
    QOpenGLVertexArrayObject VAO;
    QOpenGLTexture texture;
    QOpenGLTexture texture1;
    QTimer timer;

    QVector3D cameraPos;
    QVector3D cameraTarget;
    QVector3D cameraDirection;
    QVector3D cameraRight;
    QVector3D cameraUp;

//    Camera camera;
};


#endif // OPENGLWIDGET_H
