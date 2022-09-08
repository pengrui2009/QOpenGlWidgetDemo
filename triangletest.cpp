#include "triangletest.h"
#include <QDebug>
#include <QTime>


// 继承自QOpenGLWidget
TriangleTest::TriangleTest(QWidget *parent): QOpenGLWidget(parent)
{
    // 在初始化时进行计时
    m_factor_ = 1.0;
    m_horizintal_angle_ = 10.0;
    m_vertical_angle_ = 10.0;
    connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    timer.start(100);
}

void TriangleTest::on_timeout()
{
    // 触发绘制
    update();
}

void TriangleTest::initializeGL()
{
    // 初始化OpenGL函数
    initializeOpenGLFunctions();

    // 1.创建对象
    glGenVertexArrays(1, &VAO_id);
    glGenBuffers(1, &VBO_id);

    // 2.绑定对象
    glBindVertexArray(VAO_id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_id);

    // 3.数据存放
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_data), vertices_data, GL_STATIC_DRAW);

    // 4.解析数据
    // 1）position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 3）texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 【在VAO解绑之前进行相关代码的编写】
    // 实例化QopenGLTexture对象（已经将图片加载进来）
    textureBox = new QOpenGLTexture(QImage(":/resources/container.jpg").mirrored());

    // 5.解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 6.着色器程序
    shaderProgramObject.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/resources/shaders/triangleShader.vert");
    shaderProgramObject.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/resources/shaders/triangleShader.frag");
    shaderProgramObject.link();

    // 在着色器程序被链接成功后才能使用setUniformValue
    shaderProgramObject.bind();
    shaderProgramObject.setUniformValue("textureCity", 0);
    shaderProgramObject.setUniformValue("textureCloud", 1);

    // 【注】透视投影矩阵不用每一帧都进行设置
    QMatrix4x4 projection;
    projection.perspective(45, (float)width()/height(), 0.1, 100);
    shaderProgramObject.setUniformValue("projection", projection);
}

void TriangleTest::resizeGL(int w, int h)
{

}

void TriangleTest::paintGL()
{

    QMatrix4x4 model;
    QMatrix4x4 view;

//    static unsigned int angle = 0;
    // 设置相关矩阵
//    unsigned int time = QTime::currentTime().msec();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 绑定
    shaderProgramObject.bind();
    glBindVertexArray(VAO_id);

    // 激活0号纹理单元并将textureCity绑定上去
    textureBox->bind(0);

    model.rotate(m_horizintal_angle_, 0.0, 1.0, 0.0);
    model.rotate(m_vertical_angle_, 1.0, 0.0, 0.0);

    model.scale(m_factor_);
    view.translate(0.0, 0.0, -3); //  设置相反的结果

    shaderProgramObject.setUniformValue("model", model);
    shaderProgramObject.setUniformValue("view", view);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void TriangleTest::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0)
    {
        //ui->textEdit->zoomIn();//放大
        m_factor_ += 0.5;
    }
    else
    {
        //ui->textEdit->zoomOut();//缩小
        m_factor_ -= 0.5;
        if (m_factor_ < 0.1)
        {
            m_factor_ = 0.1;
        }
    }
}
void TriangleTest::keyPressEvent(QKeyEvent* e)
{
#if 0
    switch (e->key())
    {
    case Qt::Key_Escape:

        QCoreApplication::instance()->quit();
        break;

    case Qt::Key_Right:
        getScene()->setSideSpeed(static_cast<float>(m_cameraSpeed));
        break;

    case Qt::Key_Left:
        getScene()->setSideSpeed(static_cast<float>(-m_cameraSpeed));
        break;

    case Qt::Key_Up:
        getScene()->setForwardSpeed(static_cast<float>(m_cameraSpeed));
        break;

    case Qt::Key_Down:
        getScene()->setForwardSpeed(static_cast<float>(-m_cameraSpeed));
        break;

    case Qt::Key_PageUp:
        getScene()->setVerticalSpeed(static_cast<float>(m_cameraSpeed));
        break;

    case Qt::Key_PageDown:
        getScene()->setVerticalSpeed(static_cast<float>(-m_cameraSpeed));
        break;

    case Qt::Key_Shift:
        getScene()->setViewCenterFixed(true);
        break;

    default:
        QWindow::keyPressEvent(e);
    }
#endif
}

void TriangleTest::keyReleaseEvent(QKeyEvent* e)
{
#if 0
    switch (e->key())
    {
        case Qt::Key_Right:
        case Qt::Key_Left:
            getScene()->setSideSpeed(0.0f);
            break;

        case Qt::Key_Up:
        case Qt::Key_Down:
            getScene()->setForwardSpeed(0.0f);
            break;

        case Qt::Key_PageUp:
        case Qt::Key_PageDown:
            getScene()->setVerticalSpeed(0.0f);
            break;

        case Qt::Key_Shift:
            getScene()->setViewCenterFixed(false);
            break;

        default:
            QWindow::keyReleaseEvent(e);
    }
#endif
}

void TriangleTest::mousePressEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_leftButtonPressed = true;
        m_prevPos = e->pos();
    }

//    mousePressEvent(e);
}

void TriangleTest::mouseReleaseEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton)
    {
        m_leftButtonPressed = false;
    }

//    mouseReleaseEvent(e);
}

void TriangleTest::mouseMoveEvent(QMouseEvent* e)
{
    if(m_leftButtonPressed)
    {
        m_pos = e->pos();
        float dx = static_cast<float>(m_pos.x()) - static_cast<float>(m_prevPos.x());
        float dy = static_cast<float>(m_pos.y()) - static_cast<float>(m_prevPos.y());
        m_horizintal_angle_ += dx / 5.0;
        m_vertical_angle_ += dy / 5.0;
        qDebug() << "m_vertical_angle_:" << m_vertical_angle_;
//        float dx = static_cast<float>(m_cameraSensitivity) * (static_cast<float>(m_pos.x()) - static_cast<float>(m_prevPos.x()));
//        float dy = static_cast<float>(-m_cameraSensitivity) * (static_cast<float>(m_pos.y()) - static_cast<float>(m_prevPos.y()));

        m_prevPos = m_pos;
//        getScene()->pan(dx);
//        getScene()->tilt(dy);
    }

//    mouseMoveEvent(e);
}

