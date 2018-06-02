#ifndef HUD_H
#define HUD_H

#include <QWidget>

#include "standard/mavlink.h"
namespace Ui {
class HUD;
}

class HUD : public QWidget
{
  Q_OBJECT

public:
  explicit HUD(QWidget *parent = 0);
  ~HUD();

public slots:
  void UpdateAttitude(const mavlink_attitude_t &attitude);
  void UpdateHUD(const mavlink_vfr_hud_t &hud);
  void UpdateGPSStatus(const bool gps_status);
  void UpdateBatteryRemain(const int8_t battery_remaining);

private:
  Ui::HUD *ui;
};

#endif // HUD_H
