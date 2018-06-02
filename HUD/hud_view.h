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
  // Attitude
  void setRoll(const double roll_angle);
  void setPitch(const double pitch_angle);
  void setYaw(const double yaw_angle);

  // VFR_HUD
  void setSpeed(const double speed);
  void setGroundSpeed(const double ground_speed);
  void setAltitude(const double altitude);
  void setClimb(const double climb);
  void setHeading(const int16_t heading);
  void setThrottle(const uint16_t throttle);

  // Battery status
  void setBattery(const double battery_remain);

  // GPS
  void setGPS(const bool gps_status);
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
  double ground_speed_;
  double climb_;
  int16_t heading_;
  uint16_t throttle_;
};

#endif // HUD_VIEW_H
