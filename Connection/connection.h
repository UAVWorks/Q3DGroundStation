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
class Uav;
class ReadSerialController;
class WriteSerialController;

class Connection : public QWidget
{
  Q_OBJECT

public:
  explicit Connection(QWidget *parent = 0);
  ~Connection();

signals:
  void UpdateCmdAck(const mavlink_command_ack_t &cmd_ack);
  void UpdateBatteryRemain(const int8_t battery_remaining);
  void UpdateARM(const uint8_t base_mode);
  void UpdateMavMode(const uint8_t mav_mode);
  void UpdateAttitude(const mavlink_attitude_t &attitude);
  void UpdateAltitude(const mavlink_altitude_t &altitude);
  void UpdateAirSpeed(const float air_speed);
  void UpdateGroundSpeed(const float ground_speed);
  void UpdateHUD(const mavlink_vfr_hud_t &hud);
  void UpdateGPSStatus(const bool gps_status);
  void UpdateSysStatusSensor(const mavlink_sys_status_t &sys_status);
  void UpdateGlobalPositionInt(const mavlink_global_position_int_t &global_position);
  void UpdateAlt(const int32_t alt);

  // Don't connect slot.
  void UpdatePowerStatus(const mavlink_power_status_t &power_status);
  void UpdateNavControllerOutput(const mavlink_nav_controller_output_t &nav_ctl_out);
  void UpdateServoOutput(const mavlink_servo_output_raw_t &servo_out);
  void UpdateRawIMU(const mavlink_raw_imu_t &raw_imu);
  void WriteMessage(const QByteArray &write_data);

private slots:
  void ReadData();
  void ReadyRead();
  void TimerUpdate();
  void SendUnlockPack();
  void on_serial_connect_clicked();

  void UpdateUavInfoSlot(const mavlink_heartbeat_t &heartbeat);
  void UpdateAttitudeSlot(const mavlink_attitude_t &attitude);
  void UpdateBatteryStatusSlot(const mavlink_battery_status_t &battery);
  void UpdateSysStatusSensorSlot(const mavlink_sys_status_t &sys_status);
  void UpdateGlobalPositionIntSlot(const mavlink_global_position_int_t &global_position);
  void UpdateHUDSlot(const mavlink_vfr_hud_t &vfr_hud);
  void UpdateServoOutputSlot(const mavlink_servo_output_raw_t &servo_out);


public slots:
  void SendMAVLinkCmd(const mavlink_command_long_t *cmd_packet);
  void SendRcChannels(const mavlink_rc_channels_override_t *rc_override);
  void SendUnlockSignal(const mavlink_rc_channels_override_t *rc_unlock);
  void StopSendUnlockPack(const uint8_t base_mode);

private:
  void RequestDataStream();
  void SendHeartbeat();
  void VersionHandshaking();
  void HandleMessage(const QByteArray &read_data);
  void WriteData(const QByteArray &write_data);

private:
  Ui::Connection *ui;
  TcpClient *tcp_client_;
  QSerialPort *serial_;
  QTimer *timer_;
  QTimer *unlock_timer_;
  QUdpSocket *socket_;
  Uav *uav_;
  QByteArray unlock_pack_;
  ReadSerialController *read_control_;
  WriteSerialController *write_control_;
};

#endif // CONNECTION_H
