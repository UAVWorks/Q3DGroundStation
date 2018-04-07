#include "data_curve.h"
#include "ui_datacurve.h"

DataCurve::DataCurve(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DataCurve)
{
  ui->setupUi(this);
  ui->qvu->setLeftValue(50);
  ui->qvu->setRightValue(50);
}

DataCurve::~DataCurve()
{
  delete ui;
}
