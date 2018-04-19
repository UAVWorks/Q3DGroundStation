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
  void UpdateHUD(const mavlink_attitude_t &attitude);

private:
  Ui::HUD *ui;
};

#endif // HUD_H
