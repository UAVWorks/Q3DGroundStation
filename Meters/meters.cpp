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
  air_speed_(0.0)
{
  ui->setupUi(this);

  ui->motol_1->setScale(100);
  ui->motol_2->setScale(100);
  ui->motol_3->setScale(100);
  ui->motol_4->setScale(100);

  memset(static_cast<void *>(&attitude_), 0, sizeof(attitude_));

  // Serial request data frequency should be > Meter update frequency
  timer_->start(20);
  connect(timer_, &QTimer::timeout, this, &Meters::TimerUpdate);
}

Meters::~Meters()
{
  delete ui;
}


void Meters::UpdateAirSpeed(const float air_speed) {
  air_speed_ = air_speed;
}


void Meters::UpdateAttitude(const mavlink_attitude_t &attitude) {
  attitude_ = attitude;
}

void Meters::UpdateGPS(const mavlink_gps_raw_int_t &gps) {
  gps_ = gps;
}


void Meters::TimerUpdate() {
  ui->compass->setAngle(attitude_.yaw);
  ui->compass->update();

  ui->motol_1->setSpeed(attitude_.rollspeed);
  ui->motol_1->UpdateView();
  ui->motol_2->setSpeed(attitude_.pitchspeed);
  ui->motol_2->UpdateView();
  ui->motol_3->setSpeed(attitude_.yawspeed);
  ui->motol_3->UpdateView();
  ui->motol_4->setSpeed(attitude_.yawspeed);
  ui->motol_4->UpdateView();
}
