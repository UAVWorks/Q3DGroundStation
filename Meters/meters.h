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
  void UpdateAirSpeed(const float air_speed);
  void UpdateAttitude(const mavlink_attitude_t &attitude);
  void UpdateGPS(const mavlink_gps_raw_int_t &gps);
  //void UpdateMotor();

private:
  void TimerUpdate();

private:
  Ui::Meters *ui;
  QTimer *timer_;
  float air_speed_;
  mavlink_attitude_t attitude_;
  mavlink_gps_raw_int_t gps_;
};

#endif // METERS_H
