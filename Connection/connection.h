#ifndef CONNECTION_H
#define CONNECTION_H

#include <QWidget>

namespace Ui {
class Connection;
}

class TcpClient;
class QSerialPort;
class QTimer;
struct MspAttitudeDownDC;
struct MspMotorDownDC;
class AttitudeProtocolProcess;
class MotorProtocolProcess;

class Connection : public QWidget
{
  Q_OBJECT

public:
  explicit Connection(QWidget *parent = 0);
  ~Connection();

signals:
  void UpdateAttitude(const MspAttitudeDownDC &maddc);
  void UpdateMotor(const MspMotorDownDC &mmddc);


private slots:
  void on_connect_tcp_clicked();
  void on_serial_connect_clicked();
  void on_request_data_btn_clicked();
  void ReadData();
  void WriteData(const QByteArray &out_data);
  void TimerUpdate();

private:
  Ui::Connection *ui;
  TcpClient *tcp_client_;
  QSerialPort *serial_;
  AttitudeProtocolProcess *attitude_process_;
  MotorProtocolProcess *motor_process_;
  QTimer *timer_;
};

#endif // CONNECTION_H
