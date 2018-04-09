#include "data_curve.h"
#include "ui_datacurve.h"

DataCurve::DataCurve(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DataCurve)
{
  ui->setupUi(this);
}

DataCurve::~DataCurve()
{
  delete ui;
}
