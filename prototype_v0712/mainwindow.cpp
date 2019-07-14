#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("主页");
    showMaximized();
    init();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    openGL = new OpenGL();
    openGL->setWindowTitle(tr("Open GL"));
}

void MainWindow::on_actionOpenGL_triggered()
{
    openGL->exec();
}
//读取设置
void MainWindow::readSettings()
{
    QSettings setting("v0712", "Digit-pro");
    QPoint pos = setting.value("pos", QPoint(200, 200)).toPoint();
    QSize size = setting.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
}

//写入设置
void MainWindow::writeSettings()
{
    QSettings setting("v0712", "Digit-pro");
    setting.setValue("pos", pos());
    setting.setValue("size", size());
}
//关闭主窗口时的动作，保存应用设置
void MainWindow::closeEvent(QCloseEvent *event)
{
    //先关闭其他子窗口

    //保存设置
    writeSettings();
    event->accept();
}