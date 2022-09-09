#ifndef TRIANGLETEST_H
#define TRIANGLETEST_H

#include "modelloader.h"


#include <QWheelEvent>
// 引入相关库
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

// Qt提供的用于纹理操作相关的库
#include <QOpenGLTexture>
#include <QTimer>

// 继承自相关类
class TriangleTest : public QOpenGLWidget,QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit TriangleTest(QWidget *parent = nullptr);

private:
#if 1
    float vertices_data0[180] = {
           -0.3f, -0.3f, -0.3f,  0.0f, 0.0f,
            0.3f, -0.3f, -0.3f,  1.0f, 0.0f,
            0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
            0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
           -0.3f,  0.3f, -0.3f,  0.0f, 1.0f,
           -0.3f, -0.3f, -0.3f,  0.0f, 0.0f,

           -0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
            0.3f, -0.3f,  0.3f,  1.0f, 0.0f,
            0.3f,  0.3f,  0.3f,  1.0f, 1.0f,
            0.3f,  0.3f,  0.3f,  1.0f, 1.0f,
           -0.3f,  0.3f,  0.3f,  0.0f, 1.0f,
           -0.3f, -0.3f,  0.3f,  0.0f, 0.0f,

           -0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
           -0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
           -0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
           -0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
           -0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
           -0.3f,  0.3f,  0.3f,  1.0f, 0.0f,

            0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
            0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
            0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
            0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
            0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
            0.3f,  0.3f,  0.3f,  1.0f, 0.0f,

           -0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
            0.3f, -0.3f, -0.3f,  1.0f, 1.0f,
            0.3f, -0.3f,  0.3f,  1.0f, 0.0f,
            0.3f, -0.3f,  0.3f,  1.0f, 0.0f,
           -0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
           -0.3f, -0.3f, -0.3f,  0.0f, 1.0f,

           -0.3f,  0.3f, -0.3f,  0.0f, 1.0f,
            0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
            0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
            0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
           -0.3f,  0.3f,  0.3f,  0.0f, 0.0f,
           -0.3f,  0.3f, -0.3f,  0.0f, 1.0f
    };
#endif

    // VAO && VBO
    unsigned int VBO_id, VAO_id;

    // 创建QOpenGLShaderProgram对象
    QOpenGLShaderProgram shaderProgramObject;

    // QOpenGLTexture对象
    QOpenGLTexture * textureBox;

    QTimer timer;
    float m_factor_;
    float m_horizintal_angle_;
    float m_vertical_angle_;
    bool m_leftButtonPressed;
    QPoint m_pos;
    QPoint m_prevPos;
    float m_cameraSensitivity;
    std::vector<Point>  m_vpoints;
    std::vector<Point>  m_vnpoints;
    std::vector<Point>  m_vtpoints;

    std::vector<float> m_vertices;
public slots:
    void on_timeout();

protected:
    // 需实现的虚函数
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent* e);
    virtual void keyReleaseEvent(QKeyEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    void LoadModel(const QString &pathname);
};

#endif // TRIANGLETEST_H
