#include "value_control.h"
#include "ui_valuecontrol.h"

#include <QPainter>
#include <QMessageBox>

ValueControl::ValueControl(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ValueControl)
{
  ui->setupUi(this);
  InitControl();
}

ValueControl::~ValueControl()
{
  delete ui;
}

void ValueControl::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.end();
}

/**
 * @brief Update Attitude
 * @param attitude
 */
void ValueControl::UpdateAttitude(const mavlink_attitude_t &attitude) {
  ui->roll->setText(QString::number((180 / 3.14159) * attitude.roll, 'f', 2));
  ui->pitch->setText(QString::number((180 / 3.14159) * attitude.pitch, 'f', 2));
  ui->yaw->setText(QString::number((180 / 3.14159) * attitude.yaw, 'f', 2));
}

/**
 * @brief Update global pos
 * @param global_position
 * @note update lat, lon, alt, time_boot
 */
void ValueControl::UpdateGlobalPositionInt(const mavlink_global_position_int_t &global_position) {
  // degrees * 1E7
  ui->lat->setText(QString::number(global_position.lat / 1.0e7, 'f', 2));
  ui->lon->setText(QString::number(global_position.lon / 1.0e7, 'f', 2));
  ui->alt->setText(QString::number(global_position.alt / 10000.0, 'f', 2));
  // ms
  ui->start_time->setText(QString::number(global_position.time_boot_ms / 1000, 'f', 2));
}


/**
 * @brief Update air speed
 * @param air_speed
 */
void ValueControl::UpdateAirSpeed(const float air_speed) {
  ui->air_speed->setText(QString::number(air_speed, 'f', 2));
}

/**
 * @brief Update ground speed
 * @param ground_speed
 */
void ValueControl::UpdateGroundSpeed(const float ground_speed) {
  ui->ground_speed->setText(QString::number(ground_speed, 'f', 2));
}

/*
void ValueControl::UpdateGPSStatus(const int gps_use) {
  ui->gps_status->setText(gps_use == 1 ? QString("GPS") : QString("No GPS"));
}
*/

/**
 * @brief Update mav mode
 * @param mav_mode
 */
void ValueControl::UpdateFlightMode(const uint8_t mav_mode) {
  switch (mav_mode & 0xFF) {
    case 0:
      ui->flight_mode->setText(QString("STABILIZE"));
      break;
    case 1:
      ui->flight_mode->setText(QString("ACRO"));
      break;
    case 2:
      ui->flight_mode->setText(QString("ALT_HOLD"));
      break;
    case 3:
      ui->flight_mode->setText(QString("AUTO"));
      break;
    case 4:
      ui->flight_mode->setText(QString("GUIDED"));
      break;
    case 5:
      ui->flight_mode->setText(QString("LOITER"));
      break;
    case 6:
      ui->flight_mode->setText(QString("RTL"));
      break;
    case 7:
      ui->flight_mode->setText(QString("CIRCLE"));
      break;
    case 9:
      ui->flight_mode->setText(QString("LAND"));
      break;
    default:
      break;
  }
}

/**
 * @brief Update Arm or DisArm
 * @param cmd_ack
 */
/*
void ValueControl::UpdateARM(const mavlink_command_ack_t &cmd_ack) {
  if ((cmd_ack.command == MAV_CMD_COMPONENT_ARM_DISARM) && (cmd_ack.result == MAV_RESULT_ACCEPTED)) {
    ui->arm->setText(QString("ARM"));
  } else {
    ui->arm->setText(QString("DISARM"));
  }
}
*/

/**
 * @brief Update Arm or DisArm
 * @param cmd_ack
 */
void ValueControl::UpdateARM(const uint8_t base_mode) {
  if (base_mode & MAV_MODE_FLAG_SAFETY_ARMED) {
    ui->arm->setText(QString("ARM"));
  } else {
    ui->arm->setText(QString("DISARM"));
  }
}


void ValueControl::UpdateStartTimer(const double start_time) {
  // using global pos
}


void ValueControl::UpdateNextWP(const double next_wp) {

}


/**
 * @brief Init all control
 */
void ValueControl::InitControl(void) {
  ui->arm->setText(QString("DISARM"));
  ui->flight_mode->setText(QString("Unknown"));
  ui->start_time->setText(QString("0.00"));
  ui->next_waypoint->setText(QString("0.00"));

  ui->lat->setText(QString("0.00"));
  ui->lon->setText(QString("0.00"));
  ui->air_speed->setText(QString("0.00"));
  ui->ground_speed->setText(QString("0.00"));

  ui->alt->setText(QString("0.00"));
  ui->roll->setText(QString("0.00"));
  ui->pitch->setText(QString("0.00"));
  ui->yaw->setText(QString("0.00"));
}










