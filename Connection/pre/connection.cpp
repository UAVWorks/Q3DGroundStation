#include "connection.h"
#include "ui_connection.h"

Connection::Connection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connection),
    serial_port_(nullptr)
{
    ui->setupUi(this);
}

Connection::~Connection()
{
    delete ui;
}


void Connection::on_serial_port_clicked()
{
    // 开启串口配置非模态对话框
    if (serial_port_ != nullptr) {
        serial_port_->show();
    } else {
        serial_port_ = new SerialPort;
        //connect(serial_port_, &SerialPort::SerialPortIsOpen, this, &Connection::on_serial_port_is_open);
        connect(serial_port_, &SerialPort::SerialPortIsOpen, [=](bool serial_com_is_open){
            if (serial_com_is_open) {
                ui->serial_statu->setText(QObject::tr("UART is open"));
            } else {
                ui->serial_statu->setText(QObject::tr("UART is close"));
            }
        });

        serial_port_->show();
    }
}


/*
void Connection::on_serial_port_is_open(bool serial_com_is_open)
{
    if (serial_com_is_open) {
        ui->serial_statu->setText(QObject::tr("UART is open"));
    } else {
        ui->serial_statu->setText(QObject::tr("UART is close"));
    }
}
*/

