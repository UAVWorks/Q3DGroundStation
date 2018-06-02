#include "cmd_param.h"
#include "ui_cmdparam.h"

CmdParam::CmdParam(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CmdParam)
{
  ui->setupUi(this);
}

CmdParam::~CmdParam()
{
  delete ui;
}

void CmdParam::SetParam1Label(QString param1) {
  ui->param1Label->setText(param1);
}

void CmdParam::SetParam2Label(QString param2) {
  ui->param2Label->setText(param2);
}

void CmdParam::SetParam3Label(QString param3) {
  ui->param3Label->setText(param3);
}

void CmdParam::SetParam4Label(QString param4) {
  ui->param4Label->setText(param4);
}

void CmdParam::SetParam5Label(QString param5) {
  ui->param5Label->setText(param5);
}

void CmdParam::SetParam6Label(QString param6) {
  ui->param6Label->setText(param6);
}

void CmdParam::SetParam7Label(QString param7) {
  ui->param7Label->setText(param7);
}

void CmdParam::on_buttonBox_accepted() {
  memset((void*)(&params_), 0, sizeof(params_));

  // get line edit input
  params_.param1 = ui->param1LineEdit->text().toFloat();
  params_.param2 = ui->param2LineEdit->text().toFloat();
  params_.param3 = ui->param3LineEdit->text().toFloat();
  params_.param4 = ui->param4LineEdit->text().toFloat();
  params_.param5 = ui->param5LineEdit->text().toFloat();
  params_.param6 = ui->param6LineEdit->text().toFloat();
  params_.param7 = ui->param7LineEdit->text().toFloat();

  ui->param1LineEdit->clear();
  ui->param2LineEdit->clear();
  ui->param3LineEdit->clear();
  ui->param4LineEdit->clear();
  ui->param5LineEdit->clear();
  ui->param6LineEdit->clear();
  ui->param7LineEdit->clear();

  // send signal
  emit SendCmdParams(params_);
}









