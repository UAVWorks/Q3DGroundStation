#ifndef VALUE_CONTROL_H
#define VALUE_CONTROL_H

#include <QWidget>

#include "common/mavlink.h"

namespace Ui {
  class ValueControl;
}

class ValueControl : public QWidget
{
  Q_OBJECT

public:
  explicit ValueControl(QWidget *parent = 0);
  ~ValueControl();

protected:
  void paintEvent(QPaintEvent *event);

public slots:
  void UpdateAttitude(const mavlink_attitude_t &attitude);
  void UpdateGlobalPositionInt(const mavlink_global_position_int_t &global_position);
  void UpdateAirSpeed(const float air_speed);
  void UpdateGroundSpeed(const float ground_speed);
  void UpdateFlightMode(const uint8_t mav_mode);
  //void UpdateARM(const mavlink_command_ack_t &cmd_ack);
  void UpdateARM(const uint8_t base_mode);
  void UpdateStartTimer(const double start_time);
  void UpdateNextWP(const double next_wp);

private:
  void InitControl(void);

private:
  Ui::ValueControl *ui;
};

#endif // VALUE_CONTROL_H
