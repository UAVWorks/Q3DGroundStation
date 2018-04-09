#include "meters.h"
#include "ui_meters.h"

#include <QTimer>


#include "../MSProtocol/msp_protocol_convert_to_real_data.h"
#include "QMeter/qmeter.h"


Meters::Meters(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Meters),
  timer_(new QTimer(this))
{
  ui->setupUi(this);
  ui->qvu->setLeftValue(50);
  ui->qvu->setRightValue(50);

  // Serial request data frequency should be > Meter update frequency
  timer_->start(20);
  connect(timer_, &QTimer::timeout, this, &Meters::TimerUpdate);
}

Meters::~Meters()
{
  delete ui;
}

/*
void Meters::UpdateMeters(const MspAttitudeDownDC &maddc) {

}
*/

/*
// This ops can merge to UpdateMeters
void Meters::UpdateMotor(const MspMotorDownDC &mmddc) {
  mmddc_ = mmddc;
}
*/


void Meters::TimerUpdate() {
  /*
  ui->lt_motor->setValue(mmddc_.lt_motor);
  ui->rt_motor->setValue(mmddc_.rt_motor);
  ui->lb_motor->setValue(mmddc_.lb_motor);
  ui->rb_motor->setValue(mmddc_.rb_motor);
  */
}
