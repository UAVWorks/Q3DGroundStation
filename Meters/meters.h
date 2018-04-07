#ifndef METERS_H
#define METERS_H

#include <QWidget>

namespace Ui {
class Meters;
}

struct MspAttitudeDownDC;
struct MspMotorDownDC;

class Meters : public QWidget
{
  Q_OBJECT

public:
  explicit Meters(QWidget *parent = 0);
  ~Meters();

public slots:
  void UpdateMeters(const MspAttitudeDownDC &maddc);
  void UpdateMotor(const MspMotorDownDC &mmddc);


private:
  Ui::Meters *ui;
};

#endif // METERS_H
