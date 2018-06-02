#ifndef FLIGHT_CONTROL_H
#define FLIGHT_CONTROL_H

#include <QWidget>
#include <mutex>

#include "common/mavlink.h"
#include "q3dgs.h"


namespace Ui {
  class FlightControl;
}

class CmdParam;
class TrackPlanning;

class FlightControl : public QWidget
{
  Q_OBJECT

public:
  explicit FlightControl(QWidget *parent = 0);
  ~FlightControl();
protected:
  void paintEvent(QPaintEvent *event);

signals:
  void SendMAVLinkCmd(const mavlink_command_long_t *cmd_packet);
  void SendRcChannels(const mavlink_rc_channels_override_t *rc_override);
  void SendSetMode(const mavlink_set_mode_t *mode);
  void SendUnlockSignal(const mavlink_rc_channels_override_t *rc_unlock);

private slots:
  void on_unlock_clicked();

  void on_loiter_clicked();

  void on_rtl_clicked();

  void on_land_clicked();

  //void on_alt_clicked();

  //void on_set_mode_clicked();

  //void on_set_home_clicked();

  //void on_mission_start_clicked();

  //void on_mavlink_version_clicked();


  void on_mission_clicked();
  void on_key_ctl_clicked();

private slots:
  void ReceiveCmdParams(struct Params &params);

  void on_set_mode_clicked();

  void on_take_off_clicked();

public slots:
  void UpdateServoOutput(const mavlink_servo_output_raw_t &servo_out);

protected:
  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

private:
  Ui::FlightControl *ui;
  mavlink_command_long_t cmd_packet_;
  mavlink_rc_channels_override_t rc_override_;
  mavlink_set_mode_t set_mode_;
  CmdParam *cmd_param_;
  TrackPlanning *trask_planning_;
  struct Params params_;
  std::mutex mutex_;
};

#endif // FLIGHT_CONTROL_H
