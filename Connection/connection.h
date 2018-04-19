#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>

#include "common/mavlink.h"

namespace Ui {
class Connection;
}

class TcpClient;
class QSerialPort;
class QTimer;
class QUdpSocket;

class Connection : public QWidget
{
  Q_OBJECT

public:
  explicit Connection(QWidget *parent = 0);
  ~Connection();

signals:
  void UpdateAttitude(const mavlink_attitude_t &attitude);
  void UpdateAltitude(const mavlink_altitude_t &altitude);
  void UpdateBattery(const mavlink_battery_status_t &battery);
  void UpdateAirSpeed(const float air_speed);
  void UpdateGPS(const mavlink_gps_raw_int_t &gps);
  void UpdateSysStatusSensor(const mavlink_sys_status_t &sys_status);

  //void UpdateMotor(const MspMotorDownDC &mmddc);
  //void UpdateRC(const MspRcDownDC &mrddc);


private slots:
  //void on_connect_tcp_clicked();
  void on_serial_connect_clicked();
  void on_request_data_btn_clicked();
  void ReadData();
  void WriteData(const QByteArray &out_data);
  void TimerUpdate();
  void ReadyRead();

private:
  Ui::Connection *ui;
  TcpClient *tcp_client_;
  QSerialPort *serial_;
  QTimer *timer_;
  QUdpSocket *socket_;

};

#endif // CONNECTION_H
