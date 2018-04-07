#include "meters.h"
#include "ui_meters.h"

#include "../MSProtocol/msp_protocol_convert_to_real_data.h"
#include "QMeter/qmeter.h"


Meters::Meters(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Meters)
{
  ui->setupUi(this);

  ui->lt_motor->setForeground(Qt::white);
  ui->rt_motor->setForeground(Qt::white);
  ui->lb_motor->setForeground(Qt::white);
  ui->rb_motor->setForeground(Qt::white);

}

Meters::~Meters()
{
  delete ui;
}


void Meters::UpdateMeters(const MspAttitudeDownDC &maddc) {

}


// This ops can merge to UpdateMeters
void Meters::UpdateMotor(const MspMotorDownDC &mmddc) {
  // Debug use
  double lt = mmddc.lt_motor;
  double rt = mmddc.rt_motor;
  double lb = mmddc.lb_motor;
  double rb = mmddc.rb_motor;
  ui->lt_motor->setValue(mmddc.lt_motor);
  ui->rt_motor->setValue(mmddc.rt_motor);
  ui->lb_motor->setValue(mmddc.lb_motor);
  ui->rb_motor->setValue(mmddc.rb_motor);
}
