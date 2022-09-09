#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QFile file(":/resources/qss/lightblue.css");
//    if (file.open(QFile::ReadOnly))
//    {
//        QString qss = QLatin1String(file.readAll());
//        QString paletteColor = qss.mid(20, 7);
//        qApp->setPalette(QPalette(QColor(paletteColor)));
//        qApp->setStyleSheet(qss);
//        file.close();
//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Open Obj File"), QCoreApplication::applicationFilePath(), tr("Image Files (*.obj *.Obj *.OBJ)"));

    ui->openGLWidget->LoadModel(fileName);
    ui->openGLWidget->update();
}
