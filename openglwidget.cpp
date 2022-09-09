#include "openglwidget.h"
#include <QtMath>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , VBO(QOpenGLBuffer::VertexBuffer)
    , texture(QOpenGLTexture::Target2D)
    , texture1(QOpenGLTexture::Target2D)
    , cameraPos(0.0f,0.0f,3.0f)
    , cameraTarget(0.0f,0.0f,0.0f)
    , cameraDirection(cameraPos-cameraTarget)
    , cameraRight(QVector3D::crossProduct({0.0f,1.0f,0.0f},cameraDirection))
    , cameraUp(QVector3D::crossProduct(cameraDirection,cameraRight))
//    , camera(this)
{
    vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    cubePositions = {
      { 0.0f,  0.0f,  0.0f  },
      { 2.0f,  5.0f, -15.0f },
      {-1.5f, -2.2f, -2.5f  },
      {-3.8f, -2.0f, -12.3f },
      { 2.4f, -0.4f, -3.5f  },
      {-1.7f,  3.0f, -7.5f  },
      { 1.3f, -2.0f, -2.5f  },
      { 1.5f,  2.0f, -2.5f  },
      { 1.5f,  0.2f, -1.5f  },
      {-1.3f,  1.0f, -1.5f  },
    };

    timer.setInterval(18);
    connect(&timer,&QTimer::timeout,this,static_cast<void (OpenGLWidget::*)()>(&OpenGLWidget::update));
    timer.start();
}

OpenGLWidget::~OpenGLWidget()
{
    makeCurrent();
    texture.destroy();
    texture1.destroy();
    doneCurrent();
}

void OpenGLWidget::initializeGL()
{
    this->initializeOpenGLFunctions();        //初始化opengl函数
    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/resources/triangle.vert")){     //添加并编译顶点着色器
        qDebug()<<"ERROR:"<<shaderProgram.log();    //如果编译出错,打印报错信息
    }
    if(!shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/resources/triangle.frag")){   //添加并编译片段着色器
        qDebug()<<"ERROR:"<<shaderProgram.log();    //如果编译出错,打印报错信息
    }
    if(!shaderProgram.link()){                      //链接着色器
        qDebug()<<"ERROR:"<<shaderProgram.log();    //如果链接出错,打印报错信息
    }

    QOpenGLVertexArrayObject::Binder{&VAO};

    VBO.create();       //生成VBO对象
    VBO.bind();         //将VBO绑定到当前的顶点缓冲对象（QOpenGLBuffer::VertexBuffer）中

    //将顶点数据分配到VBO中，第一个参数为数据指针，第二个参数为数据的字节长度
    VBO.allocate(vertices.data(),sizeof(float)*vertices.size());

    texture.create();
    texture.setData(QImage(":/resources/msg_info.png").mirrored());
    texture.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
    texture.setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
    texture.setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);

    texture1.create();
    texture1.setData(QImage(":/resources/tusimple_logo.png").mirrored());
    texture1.setMinMagFilters(QOpenGLTexture::LinearMipMapLinear,QOpenGLTexture::Linear);
    texture1.setWrapMode(QOpenGLTexture::DirectionS,QOpenGLTexture::Repeat);
    texture1.setWrapMode(QOpenGLTexture::DirectionT,QOpenGLTexture::Repeat);

    //设置顶点解析格式，并启用顶点
    shaderProgram.setAttributeBuffer("aPos", GL_FLOAT, 0, 3, sizeof(GLfloat) * 5);
    shaderProgram.enableAttributeArray("aPos");
    shaderProgram.setAttributeBuffer("aTexCoord", GL_FLOAT,sizeof(GLfloat) * 3, 2, sizeof(GLfloat) * 5);
    shaderProgram.enableAttributeArray("aTexCoord");

     this->glEnable(GL_DEPTH_TEST);

//     camera.init();
}

void OpenGLWidget::resizeGL(int w, int h)
{
    this->glViewport(0,0,w,h);                //定义视口区域
}

void OpenGLWidget::paintGL()
{

    this->glClearColor(0.1f,0.5f,0.7f,1.0f);                       //设置清屏颜色
    this->glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);       //清除颜色缓存和深度缓存
    float time=QTime::currentTime().msecsSinceStartOfDay()/1000.0;
    shaderProgram.bind();                                          //使用shaderProgram着色程序
//    shaderProgram.setUniformValue("view",camera.getView());
    QMatrix4x4 projection;
    projection.perspective(45.0f,width()/(float)height(),0.1f,100.0f);
    shaderProgram.setUniformValue("projection",projection);

    texture.bind(0);                                    //将texture绑定到纹理单元0
    shaderProgram.setUniformValue("ourTexture",0);      //让ourTexture从纹理单元0中获取纹理数据

    texture1.bind(1);                                    //将texture绑定到纹理单元1
    shaderProgram.setUniformValue("ourTexture1",1);      //让ourTexture从纹理单元1中获取纹理数据
    QOpenGLVertexArrayObject::Binder{&VAO};

    for(unsigned int i = 0; i < 10; i++){
        QMatrix4x4 model;
        model.translate(cubePositions[i]);
        model.rotate(180*time+i*20.0f,QVector3D(1.0f,0.5f,0.3f));
        shaderProgram.setUniformValue("model",model);
        this->glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


bool OpenGLWidget::event(QEvent *e)
{
//    camera.handle(e);
    return QWidget::event(e);   //调用父类的事件分发函数
}
