#ifndef CMD_PARAM_H
#define CMD_PARAM_H

#include <QDialog>

#include "q3dgs.h"

namespace Ui {
  class CmdParam;
}



class CmdParam : public QDialog
{
  Q_OBJECT

public:
  explicit CmdParam(QWidget *parent = 0);
  ~CmdParam();

public:
  void SetParam1Label(QString param1);
  void SetParam2Label(QString param2);
  void SetParam3Label(QString param3);
  void SetParam4Label(QString param4);
  void SetParam5Label(QString param5);
  void SetParam6Label(QString param6);
  void SetParam7Label(QString param7);

private slots:
  void on_buttonBox_accepted();

signals:
  void SendCmdParams(struct Params &params);

private:
  Ui::CmdParam *ui;
  struct Params params_;
};

#endif // CMD_PARAM_H
