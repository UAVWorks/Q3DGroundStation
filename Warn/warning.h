#ifndef WARNING_H
#define WARNING_H

#include <QWidget>

#include "common/mavlink.h"

namespace Ui {
  class Warning;
}

class Warning : public QWidget
{
  Q_OBJECT

public:
  explicit Warning(QWidget *parent = 0);
  ~Warning();

public slots:
  void UpdateSysStatusSensor(const mavlink_sys_status_t &sys_status);
private:
  QColor GetItemColor(uint32_t present, uint32_t enabled, uint32_t health, uint32_t status_sensor_type);
private:
  Ui::Warning *ui;
};

#endif // WARNING_H
