#ifndef ATTITUDE_CONTROL_H
#define ATTITUDE_CONTROL_H

#include <QWidget>

namespace Ui {
  class AttitudeControl;
}

class QTimer;

class AttitudeControl : public QWidget
{
  Q_OBJECT

public:
  explicit AttitudeControl(QWidget *parent = 0);
  ~AttitudeControl();

protected:
  void paintEvent(QPaintEvent *event);

private slots:
  void TimerUpdate();

private:
  void DrawCircle(QPainter &painter, QTransform transform);
  void DrawCenterScale(QPainter &painter, QTransform transform);

public:
  void setRoll(const double roll_angle);
  void setPitch(const double pitch_angle);
  void UpdateView();

private:
  Ui::AttitudeControl *ui;
  double roll_angle_;
  double pitch_angle_;
  QTimer *timer_;
};

#endif // ATTITUDE_CONTROL_H
