#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include "point3d.h"

#include <QTime>
#include <QTimer>
#include <QWidget>
#include "qopenglwidget.h"
#include <qopenglfunctions.h>
#include <qopenglshaderprogram.h>
#include <QOpenGLBuffer>

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
    QTime m_time;
    int m_lastTime;
    int m_mouseEventTime;

    GLfloat m_rotation_x;
    GLfloat m_rotation_y;
    GLfloat m_rotation_z;

    QPointF m_prev_pos;
    QPointF m_curr_pos;

    float m_distance;
    Point3d m_rotation;
    Point3d m_angularMomentum;
    Point3d m_accumulatedMomentum;

    QColor m_modelColor;
    QColor m_backgroundColor;

    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;
    QOpenGLTexture* texture[2];
    GLfloat translate,xRot,yRot,zRot;
};


#endif // OPENGLWIDGET_H
