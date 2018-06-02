#include "meters.h"
#include "ui_meters.h"

#include <QTimer>
#include <QPalette>

//#include "../MSProtocol/msp_protocol_convert_to_real_data.h"
#include "QMeter/qmeter.h"


Meters::Meters(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Meters),
  timer_(new QTimer(this)),
  air_speed_(0.0),
  ground_speed_(0.0)
{
  ui->setupUi(this);

  ui->air_speed->setShowText(QString("AirSpeed"));
  ui->ground_speed->setShowText(QString("GouSpeed"));
  ui->roll1_pitch2->setLeftLabel(QString("r"));
  ui->roll1_pitch2->setRightLabel(QString("p"));
  ui->throttle3_yaw4->setLeftLabel(QString("t"));
  ui->throttle3_yaw4->setRightLabel(QString("y"));
  ui->alt->setAlt(0.0);
  ui->vsi->setvis(0.0);

  memset(static_cast<void *>(&attitude_), 0, sizeof(attitude_));
  memset(static_cast<void *>(&rc_channels_), 0, sizeof(rc_channels_));
  memset(static_cast<void *>(&vfr_hud_), 0, sizeof(vfr_hud_));

  servo_out_.servo1_raw = 1000;
  servo_out_.servo2_raw = 1000;
  servo_out_.servo3_raw = 1000;
  servo_out_.servo4_raw = 1000;

  alt_ = 0.0;

  // Serial request data frequency should be > Meter update frequency
  timer_->start(20);
  connect(timer_, &QTimer::timeout, this, &Meters::TimerUpdate);
}

Meters::~Meters()
{
  delete ui;
}


void Meters::UpdateHUD(const mavlink_vfr_hud_t &vfr_hud) {
  vfr_hud_ = vfr_hud;
}


void Meters::UpdateAttitude(const mavlink_attitude_t &attitude) {
  attitude_ = attitude;
}

void Meters::UpdateServoOutput(const mavlink_servo_output_raw_t &servo_out) {
  servo_out_ = servo_out;
}

void Meters::UpdateAlt(const int32_t alt) {
  alt_ = alt;
}


void Meters::TimerUpdate() {
  ui->compass->setAngle((180 / 3.14159) * attitude_.yaw);
  ui->compass->update();

  ui->alt->setAlt(alt_ / 10000.0);
  ui->alt->UpdateView();

  ui->vsi->setvis(vfr_hud_.climb);
  ui->vsi->UpdateView();

  ui->air_speed->setSpeed(vfr_hud_.airspeed);
  ui->air_speed->UpdateView();

  ui->ground_speed->setSpeed(vfr_hud_.groundspeed);
  ui->ground_speed->UpdateView();

  ui->attitude->setRoll((180 / 3.14159) * attitude_.roll);
  ui->attitude->setPitch((180 / 3.14159) * attitude_.pitch);
  ui->attitude->UpdateView();

  ui->roll1_pitch2->setLeftValue(((servo_out_.servo1_raw - 1000) / 1000.0) * 100.0);
  ui->roll1_pitch2->setRightValue(((servo_out_.servo2_raw - 1000) / 1000.0) * 100.0);
  ui->roll1_pitch2->update();

  ui->throttle3_yaw4->setLeftValue(((servo_out_.servo3_raw - 1000) / 1000.0) * 100);
  ui->throttle3_yaw4->setRightValue(((servo_out_.servo4_raw - 1000) / 1000.0) * 100);
  ui->throttle3_yaw4->update();


}
