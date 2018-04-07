#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//#include "SerialPort/serial_port.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //void on_serial_com_clicked();

private:
    Ui::MainWindow *ui_;
    //SerialPort *seria_port_;
};

#endif // MAINWINDOW_H
