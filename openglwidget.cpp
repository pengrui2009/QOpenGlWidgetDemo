#include "openglwidget.h"
#include<qopenglshaderprogram.h>
#include<QOpenGLTexture>
#include<qimage.h>
#include<QKeyEvent>
#include <QPainter>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    translate=-6.0;
    xRot=zRot=0.0;
    yRot=-30.0;

    m_rotation_x = 30;
    m_rotation_y = 0;
    m_rotation_z = 0;

    LoadModel("/home/pengrui/workspace/qt/QOpenGLWidgetDemo/models/qt.obj");

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
    m_timer.start(1000);
}

void OpenGLWidget::slot_timeout()
{
    m_rotation_x += 0.5;
    update();
}

void OpenGLWidget::LoadModel(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;

    Point3d boundsMin( 1e9, 1e9, 1e9);
    Point3d boundsMax(-1e9,-1e9,-1e9);

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString input = in.readLine();
        if (input.isEmpty() || input[0] == '#')
            continue;

        QTextStream ts(&input);
        QString id;
        ts >> id;
        if (id == "v") {
            Point3d p;
            for (int i = 0; i < 3; ++i) {
                ts >> p[i];
                boundsMin[i] = qMin(boundsMin[i], p[i]);
                boundsMax[i] = qMax(boundsMax[i], p[i]);
            }
            m_points << p;
        } else if (id == "f" || id == "fo") {
            QVarLengthArray<int, 4> p;

            while (!ts.atEnd()) {
                QString vertex;
                ts >> vertex;
                const int vertexIndex = vertex.split('/').value(0).toInt();
                if (vertexIndex)
                    p.append(vertexIndex > 0 ? vertexIndex - 1 : m_points.size() + vertexIndex);
            }

            for (int i = 0; i < p.size(); ++i) {
                const int edgeA = p[i];
                const int edgeB = p[(i + 1) % p.size()];

                if (edgeA < edgeB)
                    m_edgeIndices << edgeA << edgeB;
            }

            for (int i = 0; i < 3; ++i)
                m_pointIndices << p[i];

            if (p.size() == 4)
                for (int i = 0; i < 3; ++i)
                    m_pointIndices << p[(i + 2) % 4];
        }
    }

    const Point3d bounds = boundsMax - boundsMin;
    const qreal scale = 1 / qMax(bounds.x, qMax(bounds.y, bounds.z));
    for (int i = 0; i < m_points.size(); ++i)
        m_points[i] = (m_points[i] - (boundsMin + bounds * 0.5)) * scale;

    m_normals.resize(m_points.size());
    for (int i = 0; i < m_pointIndices.size(); i += 3) {
        const Point3d a = m_points.at(m_pointIndices.at(i));
        const Point3d b = m_points.at(m_pointIndices.at(i+1));
        const Point3d c = m_points.at(m_pointIndices.at(i+2));

        const Point3d normal = cross(b - a, c - a).normalize();

        for (int j = 0; j < 3; ++j)
            m_normals[m_pointIndices.at(i + j)] += normal;
    }

    for (int i = 0; i < m_normals.size(); ++i)
        m_normals[i] = m_normals[i].normalize();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
#if 0
    texture[0]=new QOpenGLTexture(QImage(QString(":/myopengl/side%1.bmp")));
    texture[1]=new QOpenGLTexture(QImage(QString(":/myopengl/side%2.bmp")));
    QOpenGLShader *vshader=new QOpenGLShader(QOpenGLShader::Vertex,this);
    const char *vsrc=
       "#version 150                               \n"
       "in vec4 vPosition;                         \n"
       "in vec4 vTexCoord;                         \n"
       "out vec4 texCoord;                         \n"
       "uniform mat4 matrix;                       \n"
       "void main(){                               \n"
       "texCoord=vTexCoord;                        \n"
       "gl_Position=matrix*vPosition;              \n"
       "}                                          \n";
    if (!vshader->compileSourceCode(vsrc))
    {
        return;
    }
    QOpenGLShader *fshader=new QOpenGLShader(QOpenGLShader::Fragment,this);
    const char *fsrc=
       "#version 150                              \n"
       "uniform sampler2D tex;                    \n"
       "in vec4 texCoord;                         \n"
       "out vec4 fColor;                          \n"
       "void main(){                              \n"
       "fColor=texture(tex,vec2(texCoord));       \n"
       "}                                         \n";
    if (!fshader->compileSourceCode(fsrc))
    {
        return;
    }
    program=new QOpenGLShaderProgram(this);
    if (!program->addShader(vshader))
    {
        return;
    }
    if (!program->addShader(fshader))
    {
        return;
    }
    if (!program->link())
    {
        return;
    }
    if (!program->bind())
    {
        return;
    }
#endif
}

void OpenGLWidget::resizeGL(int w, int h)
{

}

//void OpenGLWidget::paintEvent(QPaintEvent *e)
//{
//    QPainter painter(this);;

//    QRadialGradient gradient(40, 40, 40, 40, 40);
//    gradient.setColorAt(0.2, Qt::yellow);
//    gradient.setColorAt(1, Qt::transparent);
//    painter.setBrush(gradient);
//    painter.drawRect(-width(),-height(), width()*2, height()*2);
//}

void OpenGLWidget::paintGL()
{
    int w=width();
    int h=height();
    int side=qMin(w,h);

    QPainter painter(this);

//    painter.setBrush(Qt::black);
//    painter.drawRect(-width(), -height(), width()*2, height()*2);

    glViewport((w-side)/2,(h-side)/2,side,side);
    glClear(GL_COLOR_BUFFER_BIT|GL_ACCUM_BUFFER_BIT);
#if 0
    GLfloat vertices[2][4][3]=
    {
        {
            {-0.8f,0.8f,0.8f},
            {-0.8f,-0.8f,0.8f},
            {0.8f,-0.8f,0.8f},
            {0.8f,0.8f,0.8f}
        },
        {
            {0.8f,0.8f,0.8f},
            {0.8f,-0.8f,0.8f},
            {0.8f,-0.8f,-0.8f},
            {0.8f,0.8f,-0.8f}
        },
    };
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices,48*sizeof(GLfloat));
    GLuint vPosition =program->attributeLocation("vPosition");
    //glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(vPosition);
    program->setAttributeBuffer(vPosition,GL_FLOAT,0,3,0);
    GLfloat coords[2][4][2]=
    {
        {
            {0.0f,1.0f},
            {0.0f,0.0f},
            {1.0f,0.0f},
            {1.0f,1.0f}
        },
        {
            {0.0f,1.0f},
            {0.0f,0.0f},
            {1.0f,0.0f},
            {1.0f,1.0f}
        }
    };
    vbo.write(24*sizeof (GLfloat),coords,16*sizeof(GLfloat));
    GLuint vTexCoord=program->attributeLocation("vTexCoord");
    program->setAttributeBuffer(vTexCoord,GL_FLOAT,24*sizeof (GLfloat),2,0);
    glEnableVertexAttribArray(vTexCoord);
    program->setUniformValue("tex",0);
    //透视投影效果
    QMatrix4x4 matrix;
    matrix.perspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);
    matrix.translate(0,0,translate);
    matrix.rotate(xRot,1.0,0.0,0.0);
    matrix.rotate(yRot,0.0,1.0,0.0);
    matrix.rotate(zRot,0.0,0.0,1.0);
    program->setUniformValue("matrix",matrix);
    for(int i=0;i<2;i++)
    {
        texture[i]->bind();
        glDrawArrays(GL_TRIANGLE_FAN, i*4, 4);
    }
    //    glDrawArrays(GL_POINTS,0,1);
#endif
    glEnable(GL_MULTISAMPLE);
    bool normals = false;
    bool wireframe = false;
    if (wireframe) {
        glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());
        glDrawElements(GL_LINES, m_edgeIndices.size(), GL_UNSIGNED_INT, m_edgeIndices.data());
    } else {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_COLOR_MATERIAL);
        glShadeModel(GL_SMOOTH);

        glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, (float *)m_points.data());
        glNormalPointer(GL_FLOAT, 0, (float *)m_normals.data());
        glDrawElements(GL_TRIANGLES, m_pointIndices.size(), GL_UNSIGNED_INT, m_pointIndices.data());

        glDisableClientState(GL_NORMAL_ARRAY);
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
    }

    if (normals) {
        QVector<Point3d> normals;
        for (int i = 0; i < m_normals.size(); ++i)
            normals << m_points.at(i) << (m_points.at(i) + m_normals.at(i) * 0.02f);
        glVertexPointer(3, GL_FLOAT, 0, (float *)normals.data());
        glDrawArrays(GL_LINES, 0, normals.size());
    }

    glRotatef(m_rotation_x, 1, 0, 0);
    glRotatef(m_rotation_y, 0, 1, 0);
    glRotatef(m_rotation_z, 0, 0, 1);

//    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_MULTISAMPLE);


}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
        xRot+=10;
        break;
    case Qt::Key_Left:
            yRot+=10;
            break;
    case Qt::Key_Right:
            zRot+=10;
            break;
    case Qt::Key_Down:
            translate-=1;
            break;
    case Qt::Key_Space:
            translate+=1;
            break;
    default:
        break;
    }
    update();
    QOpenGLWidget::keyPressEvent(event);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{

    if (event->isAccepted())
        return;
    if (event->buttons() & Qt::LeftButton) {
        const QPointF delta = event->pos();
        const Point3d angularImpulse = Point3d(delta.y(), delta.x(), 0) * 0.1;

//        m_rotation += angularImpulse;
//        m_accumulatedMomentum += angularImpulse;

//        event->accept();
        update();
    }
    QOpenGLWidget::mouseMoveEvent(event);
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{

    if (event->isAccepted())
        return;

//    m_mouseEventTime = m_time.elapsed();
//    m_angularMomentum = m_accumulatedMomentum = Point3d();
    event->accept();
    QOpenGLWidget::mousePressEvent(event);
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->isAccepted())
        return;

//    const int delta = m_time.elapsed() - m_mouseEventTime;
//    m_angularMomentum = m_accumulatedMomentum * (1000.0 / qMax(1, delta));
//    event->accept();
    update();
    QOpenGLWidget::mouseReleaseEvent(event);
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->isAccepted())
        return;

//    m_distance *= qPow(1.2, -event->delta() / 120);
//    event->accept();
    update();
    QOpenGLWidget::wheelEvent(event);
}

