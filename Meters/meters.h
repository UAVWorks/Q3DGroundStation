#ifndef METERS_H
#define METERS_H

#include <QWidget>


namespace Ui {
class Meters;
}

class QTimer;

class Meters : public QWidget
{
  Q_OBJECT

public:
  explicit Meters(QWidget *parent = 0);
  ~Meters();

public slots:
  //void UpdateMeters(const MspAttitudeDownDC &maddc);
  //void UpdateMotor(const MspMotorDownDC &mmddc);

private:
  void TimerUpdate();

private:
  Ui::Meters *ui;
  QTimer *timer_;
};

#endif // METERS_H
