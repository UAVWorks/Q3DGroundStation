#ifndef HUD_H
#define HUD_H

#include <QWidget>

namespace Ui {
class HUD;
}

struct MspAttitudeDownDC;

class HUD : public QWidget
{
  Q_OBJECT

public:
  explicit HUD(QWidget *parent = 0);
  ~HUD();

public slots:
  void UpdateHUD(const MspAttitudeDownDC &maddc);

private:
  Ui::HUD *ui;
};

#endif // HUD_H
