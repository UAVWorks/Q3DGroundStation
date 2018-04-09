#include "connection.h"
#include "ui_connection.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QTimer>

#include "../Tcp/tcp_client.h"

Connection::Connection(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Connection),
  tcp_client_(nullptr),
  serial_(new QSerialPort(this)),
  timer_(new QTimer(this))
{
  ui->setupUi(this);

  // set default ip, port
  ui->tcp_ip->setText(QObject::tr("127.0.0.1"));
  ui->tcp_port->setText(QObject::tr("10002"));

  // set default serial port(COM3) and bps(115200)
  ui->serialport_select->setCurrentIndex(2);
  ui->bps_select->setCurrentIndex(0);

  connect(serial_, &QSerialPort::readyRead, this, &Connection::ReadData);
  connect(timer_, &QTimer::timeout, this, &Connection::TimerUpdate);
}

Connection::~Connection()
{
  delete ui;
  // must be release point
  if (tcp_client_ != nullptr) {
    delete tcp_client_;
  }

  if (serial_ != nullptr) {
    delete serial_;
  }

}


/****************************************************
* 函数名: on_connect_tcp_clicked
* 功能: connect tcp msg source
* 参数: void
* 返回值: void
* 备注: 127.0.01, 10002
* 时间: 2018/4/4 陈登龙
****************************************************/
void Connection::on_connect_tcp_clicked()
{
  if (tcp_client_ == nullptr) {
    tcp_client_ = new TcpClient;
  }

  if (tcp_client_->IsTcpConnecting()) {
    // TCP 已经连接
    tcp_client_->StopTcpConnect();
    ui->connect_tcp->setText(QObject::tr("connect"));
  } else {
    // TCP 没有连接
    tcp_client_->SetConnectAddr(ui->tcp_port->text().toShort(), ui->tcp_ip->text().toStdString());
    tcp_client_->StartTcpConnect();
    if (tcp_client_->IsTcpConnecting()) {
      ui->connect_tcp->setText(QObject::tr("disconnect"));
      QMessageBox::information(NULL, "Tcp Info", "Tcp connect success.", QMessageBox::Yes);
    } else {
      QMessageBox::information(NULL, "Tcp Info", "Tcp connect fail!!!", QMessageBox::Yes);
    }
  }
}


/****************************************************
* 函数名: on_serial_connect_clicked
* 功能: connect serial
* 参数: void
* 返回值: void
* 备注: COM3, 115200
* 时间: 2018/4/4 陈登龙
****************************************************/
void Connection::on_serial_connect_clicked()
{
  if (ui->serial_connect->text() == QObject::tr("disconnect")) {
    // close serial
    ui->serial_connect->setText(QObject::tr("serial\nconnect"));
    serial_->close();
    QMessageBox::information(NULL, "Serial Info", "Serial close success.", QMessageBox::Yes);
  } else {
    // set com
    serial_->setPortName(ui->serialport_select->currentText());
    // set bps
    serial_->setBaudRate(ui->bps_select->currentText().toInt());
    // set data bit = 8 bit
    serial_->setDataBits(QSerialPort::Data8);
    // set parity = no
    serial_->setParity(QSerialPort::NoParity);
    // set flow ctl = no
    serial_->setFlowControl(QSerialPort::NoFlowControl);
    // set stop bit = one
    serial_->setStopBits(QSerialPort::OneStop);
    // open serial port
    if (serial_->open(QIODevice::ReadWrite)) {
      QMessageBox::information(NULL, "Serial Info", "Serial open success.", QMessageBox::Yes);
      ui->serial_connect->setText(QObject::tr("disconnect"));
    } else {
      QMessageBox::information(NULL, "Serial Info", "Serial open fail.", QMessageBox::Yes);
    }
  }
}


/***********************************************************************
* 函数名: ReadData
* 功能: read serial data
* 参数: void
* 返回值: void
* 备注: This slot fun connect QSerialPort::readyRead signal
* 时间: 2018/4/4 陈登龙
***********************************************************************/
void Connection::ReadData() {
  QByteArray in_data = serial_->readAll();
/*
  MessageHeader msg_header;

  // get msg header
  MSProtocolProcessInterface::GetMessageHeaderInfo(in_data, msg_header);

  // handle data
  switch (msg_header.command) {
    case MSP_ATTITUDE:
      // unpack
      attitude_process_->UnPack(in_data);
      // convert to real data
      attitude_process_->MspAttitudeDownToDC();
      // emit signal to update UI
      emit UpdateAttitude(attitude_process_->attitude_dc_);
      break;
    case MSP_MOTOR:
      motor_process_->UnPack(in_data);
      motor_process_->MspMotorDownToDC();
      emit UpdateMotor(motor_process_->motor_down_dc);
      break;
    case MSP_RC:
      rc_process_->UnPack(in_data);
      rc_process_->MspRcDownToDC();
      emit UpdateRC(rc_process_->rc_down_dc_);
    default:
      break;
  }
*/
}


/********************************************************
* 函数名: WriteData
* 功 能: write byte array data to serial
* 参 数: QByteArray
* 返回值: void
* 备 注: Must be byte array
* 时 间: 2018/4/4 陈登龙
*********************************************************/
void Connection::WriteData(const QByteArray &out_data) {
  serial_->write(out_data);

  qDebug() << "byteToWrite: " << serial_->bytesToWrite();
}


void Connection::on_request_data_btn_clicked()
{
  if (ui->request_data_btn->text() == QObject::tr("Request Data")) {
    // Serial request data frequency should be > Meter update frequency
    timer_->start(100);
    ui->request_data_btn->setText(QObject::tr("Stop Request"));
  } else {
    timer_->stop();
    ui->request_data_btn->setText(QObject::tr("Request Data"));
  }
}


/**
* Function name:
* Function:
* Params: void
* Return: void
* Note:
* Time: 2018/4/6 cdeveloper
*/
void Connection::TimerUpdate() {
/*
  // send data to serial
  QByteArray out_data;

  // pack attitude
  attitude_process_->Pack(out_data);
  WriteData(out_data);

  // pack motor
  motor_process_->Pack(out_data);
  WriteData(out_data);

  // pack rc
  //rc_process_->Pack(out_data);
  //WriteData(out_data);
*/
}








