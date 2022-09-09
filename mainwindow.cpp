#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
