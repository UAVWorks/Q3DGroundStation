#include "connection.h"
#include "ui_connection.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QTimer>
#include <QUdpSocket>
//#include "../Tcp/tcp_client.h"


#define SERIAL_READ_ALL_ONCE

Connection::Connection(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Connection),
  //tcp_client_(nullptr),
  serial_(new QSerialPort(this)),
  timer_(new QTimer(this)),
  socket_(new QUdpSocket(this))
{
  ui->setupUi(this);

  ui->heartbeat_label->setText(QObject::tr("0"));

  // set default ip, port
  ui->tcp_ip->setText(QObject::tr("127.0.0.1"));
  ui->tcp_port->setText(QObject::tr("10002"));

  // set default serial port(COM3) and bps(115200)
  ui->serialport_select->setCurrentIndex(2);
  ui->bps_select->setCurrentIndex(0);

  connect(serial_, &QSerialPort::readyRead, this, &Connection::ReadData);
  connect(timer_, &QTimer::timeout, this, &Connection::TimerUpdate);

  socket_->bind(QHostAddress::LocalHost, 10002);
  connect(socket_, SIGNAL(readyRead()),this, SLOT(ReadyRead()));
}

Connection::~Connection()
{
  delete ui;
  // must be release point
  /*
  if (tcp_client_ != nullptr) {
    delete tcp_client_;
  }
  */

  if (serial_ != nullptr) {
    delete serial_;
  }

}


/**
 * @brief Tcp connect
 *
 * @note 127.0.0.1, 10002
 */
/*
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
*/


/**
 * @brief Init serial
 *
 * @note COM3, 115200
 */
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


/**
 * @brief Read serial data
 *
 * @note This slot fun connect QSerialPort::readyRead signal
 */
void Connection::ReadData() {
  mavlink_message_t msg;
  mavlink_status_t status;

#ifdef SERIAL_READ_ALL_ONCE
  // read all serial data.
  QByteArray in_data = serial_->readAll();

  for (int i = 0; i < in_data.length(); i++) {
    // What is MAVLINK_COMM_0 ?
    if (mavlink_parse_char(MAVLINK_COMM_0, in_data[i], &msg, &status)) {
      switch (msg.msgid) {
        // Heartbeat
        case MAVLINK_MSG_ID_HEARTBEAT:
          mavlink_heartbeat_t heartbeat;
          // decode MAVLink msg
          mavlink_msg_heartbeat_decode(&msg, &heartbeat);
          break;
        // Attitude
        case MAVLINK_MSG_ID_ATTITUDE:
          mavlink_attitude_t attitude;
          mavlink_msg_attitude_decode(&msg, &attitude);
          // emit signal to update UI
          emit UpdateAttitude(attitude);
        // Attitude
        case MAVLINK_MSG_ID_ALTITUDE:
          mavlink_altitude_t altitude;
          mavlink_msg_altitude_decode(&msg, &altitude);
          emit UpdateAltitude(altitude);
        // Battery
        case MAVLINK_MSG_ID_BATTERY_STATUS:
          mavlink_battery_status_t battery;
          mavlink_msg_battery_status_decode(&msg, &battery);
          emit UpdateBattery(battery);
          break;
        // Control system state
        case MAVLINK_MSG_ID_CONTROL_SYSTEM_STATE:
          mavlink_control_system_state_t ctl_sys_state;
          mavlink_msg_control_system_state_decode(&msg, &ctl_sys_state);
          //emit UpdateCtlSysState(ctl_sys_state);
        default:
          break;
      }
    }
  }
#else
  uint8_t read_byte = 0;
  while (serial_->bytesAvailable() > 0) {
    if (serial_->read(&read_byte, 1) > 0) {
      // What is MAVLINK_COMM_0 ?
      if (mavlink_parse_char(MAVLINK_COMM_0, read_byte, &msg, &status)) {
        switch (msg.msgid) {
          case MAVLINK_MSG_ID_HEARTBEAT:
            mavlink_heartbeat_t heartbeat;
            mavlink_msg_heartbeat_decode(&msg, &heartbeat);
            // emit signal to update UI
            break;
          default:
            break;
        }
      }
    }
  }
#endif


}


/**
 * @brief Write byte array data to serial
 */
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
 * @brief Read udp data
 * @note receive ok
 */
void Connection::ReadyRead() {
  int read_length = socket_->pendingDatagramSize();
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  QHostAddress sender;
  uint16_t senderPort;
  // read msg
  socket_->readDatagram((char*)buf, read_length, &sender, &senderPort);

  // decoding
  mavlink_message_t msg;
  mavlink_status_t status;
  static int heartbeat_time = 0;
#ifdef SERIAL_READ_ALL_ONCE
  for (int i = 0; i < read_length; i++) {
    // What is MAVLINK_COMM_0 ?
    if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) {
      switch (msg.msgid) {
        // Heartbeat
        case MAVLINK_MSG_ID_HEARTBEAT:
          mavlink_heartbeat_t heartbeat;
          // decode MAVLink msg
          mavlink_msg_heartbeat_decode(&msg, &heartbeat);
          ui->heartbeat_label->setText(QString::number(++heartbeat_time));
          break;
        // Attitude
        case MAVLINK_MSG_ID_ATTITUDE:
          mavlink_attitude_t attitude;
          mavlink_msg_attitude_decode(&msg, &attitude);
          // emit signal to update UI
          emit UpdateAttitude(attitude);
        // Attitude
        case MAVLINK_MSG_ID_ALTITUDE:
          mavlink_altitude_t altitude;
          mavlink_msg_altitude_decode(&msg, &altitude);
          emit UpdateAltitude(altitude);
        // Battery
        case MAVLINK_MSG_ID_BATTERY_STATUS:
          mavlink_battery_status_t battery;
          mavlink_msg_battery_status_decode(&msg, &battery);
          emit UpdateBattery(battery);
          break;
        // Control system state
        case MAVLINK_MSG_ID_CONTROL_SYSTEM_STATE:
          mavlink_control_system_state_t ctl_sys_state;
          mavlink_msg_control_system_state_decode(&msg, &ctl_sys_state);
          emit UpdateAirSpeed(ctl_sys_state.airspeed);
        // GPS
        case MAVLINK_MSG_ID_GPS_RAW_INT:
          mavlink_gps_raw_int_t gps;
          mavlink_msg_gps_raw_int_decode(&msg, &gps);
          emit UpdateGPS(gps);
        case MAVLINK_MSG_ID_SYS_STATUS:
          mavlink_sys_status_t sys_status;
          mavlink_msg_sys_status_decode(&msg, &sys_status);
          emit UpdateSysStatusSensor(sys_status);
        default:
          break;
      }
    }
  }
#else
  uint8_t read_byte = 0;
  while (serial_->bytesAvailable() > 0) {
    if (serial_->read(&read_byte, 1) > 0) {
      // What is MAVLINK_COMM_0 ?
      if (mavlink_parse_char(MAVLINK_COMM_0, read_byte, &msg, &status)) {
        switch (msg.msgid) {
          case MAVLINK_MSG_ID_HEARTBEAT:
            mavlink_heartbeat_t heartbeat;
            mavlink_msg_heartbeat_decode(&msg, &heartbeat);
            // emit signal to update UI
            break;
          default:
            break;
        }
      }
    }
  }
#endif



}


/**
 * @brief Timer Update
 * @note This is tmp test code
 */
void Connection::TimerUpdate() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  uint8_t system_id = 2;
  uint8_t component_id = 1;

  //mavlink_msg_param_request_read_pack(system_id, component_id, &msg, 1, 1, "SYSID_SW_TYPE", -1);
  //uint8_t len = mavlink_msg_to_send_buffer(buf, &msg);

  mavlink_heartbeat_t heartbeat;
  // init heartbeat
  /*
  heartbeat.mavlink_version = ;
  heartbeat.autopilot = ;
  heartbeat.base_mode = ;
  heartbeat.custom_mode = ;
  heartbeat.system_status = ;
  heartbeat.type = ;
  */

  // encode MAVLink msg, this msg need to be send to UAV every 1 s
  mavlink_msg_heartbeat_encode(system_id, component_id, &msg, &heartbeat);
  // send to buf
  uint8_t len = mavlink_msg_to_send_buffer(buf, &msg);

  // copy to byte array from buf
  QByteArray out_data;
  out_data.clear();
  for (int i = 0; i < len; i++) {
    out_data.append(buf[i]);
  }

  // I don`t this function whether to be used
  // QByteArray::fromRawData(buf, len);

  WriteData(out_data);
}








