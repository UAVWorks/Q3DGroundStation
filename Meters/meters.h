#ifndef METERS_H
#define METERS_H

#include <QWidget>

#include "common/mavlink.h"

namespace Ui {
class Meters;
}

class QTimer;

class Meters : public QWidget
{
  Q_OBJECT

public:
  explicit Meters(QWidget *parent = 0);
  ~Meters();

public slots:
  void UpdateAttitude(const mavlink_attitude_t &attitude);
  void UpdateServoOutput(const mavlink_servo_output_raw_t &servo_out);
  void UpdateHUD(const mavlink_vfr_hud_t &vfr_hud);
  void UpdateAlt(const int32_t alt);
private:
  void TimerUpdate();

private:
  Ui::Meters *ui;
  QTimer *timer_;
  float air_speed_;
  float ground_speed_;
  mavlink_attitude_t attitude_;
  mavlink_gps_raw_int_t gps_;
  mavlink_rc_channels_t rc_channels_;
  mavlink_servo_output_raw_t servo_out_;
  mavlink_vfr_hud_t vfr_hud_;
  int32_t alt_;
};

#endif // METERS_H
