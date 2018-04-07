#ifndef METERS_H
#define METERS_H

#include <QWidget>

#include "../MSProtocol/msp_protocol_convert_to_real_data.h"

namespace Ui {
class Meters;
}

class QTimer;

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
  void TimerUpdate();

private:
  Ui::Meters *ui;
  QTimer *timer_;
  MspMotorDownDC mmddc_;
};

#endif // METERS_H
