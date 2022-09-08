#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "point3d.h"

#include <QTimer>
#include <QWidget>
#include"qopenglwidget.h"
#include<qopenglfunctions.h>
#include<qopenglshaderprogram.h>
#include<QOpenGLBuffer>
class QOpenGLShaderProgram;
class QOpenGLTexture;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = nullptr);

//signals:

public slots:
    void slot_timeout();

public:
    void LoadModel(const QString &filename);
//    void paintEvent(QPaintEvent *e) override;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void keyPressEvent(QKeyEvent*event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    QString m_fileName;
    QVector<Point3d> m_points;
    QVector<Point3d> m_normals;
    QVector<int> m_edgeIndices;
    QVector<int> m_pointIndices;

    QTimer m_timer;

    GLfloat m_rotation_x;
    GLfloat m_rotation_y;
    GLfloat m_rotation_z;

    QPointF m_prev_pos;
    QPointF m_curr_pos;


    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;
    QOpenGLTexture* texture[2];
    GLfloat translate,xRot,yRot,zRot;
};


#endif // OPENGLWIDGET_H
