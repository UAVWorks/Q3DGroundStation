#include "flight_control.h"
#include "ui_flightcontrol.h"

#include <QPainter>

FlightControl::FlightControl(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FlightControl)
{
  ui->setupUi(this);
}

FlightControl::~FlightControl()
{
  delete ui;
}


void FlightControl::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);
  painter.drawRect(rect());
  painter.end();
}
