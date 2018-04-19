#ifndef HUD_VIEW_H
#define HUD_VIEW_H

#include <QWidget>

namespace Ui {
  class HUDView;
}

class QTimer;

class HUDView : public QWidget
{
  Q_OBJECT

public:
  explicit HUDView(QWidget *parent = 0);
  ~HUDView();

protected:
  void paintEvent(QPaintEvent *event);
  void TimerUpdate();

private:
  void DrawHudBgColor(QPainter &painter, QTransform transform);
  void DrawHudRollRadian(QPainter &painter, QTransform transform);
  void DrawHudPitchScale(QPainter &painter, QTransform transform);
  void DrawHudCrossbow(QPainter &painter, QTransform transform);
  void DrawHudYawScale(QPainter &painter, QTransform transform);
  void DrawHudSpeed(QPainter &painter, QTransform transform);
  void DrawHudAltitude(QPainter &painter, QTransform transform);
  void DrawHudStatus(QPainter &painter, QTransform transform);

public:
  void setRoll(const double roll_angle);
  void setPitch(const double pitch_angle);
  void setYaw(const double yaw_angle);
  void setSpeed(const double speed);
  void setAltitude(const double altitude);
  void UpdateView();
private:
  Ui::HUDView *ui;
  QTimer *timer_;
  double roll_angle_;
  double pitch_angle_;
  double yaw_angle_;
  double speed_;
  double altitude_;
  bool gps_status_;
  double battery_remain_;
};

#endif // HUD_VIEW_H
