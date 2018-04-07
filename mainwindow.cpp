#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui_;
    // 主界面的子部件会自动销毁
    //delete seria_port_;
}

/*
void MainWindow::on_serial_com_clicked()
{
    //boost::scoped_ptr<SerialPort> p(new SerialPort);
    seria_port_ = new SerialPort;
    seria_port_->show();
}
*/



