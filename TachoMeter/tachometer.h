#ifndef TACHOMETER_H
#define TACHOMETER_H

#include <QWidget>

class QTimer;

namespace Ui {
  class Tachometer;
}

class Tachometer : public QWidget
{
  Q_OBJECT

public:
  explicit Tachometer(QWidget *parent = 0);
  ~Tachometer();

protected:
  void paintEvent(QPaintEvent *event);

private:
  void TimerUpdate();

private:
  void DrawStaticBarkGround(QPainter &painter);
  void DrawDynamicPointer(QPainter &painter, QTransform transform);

public:
  void setSpeed(double speed);
  void setScale(int scale);
  void setShowText(QString show_text);
  void UpdateView();

private:
  Ui::Tachometer *ui;
  QTimer *timer_;
  QString show_text_;
  double speed_;
  int scale_;
};

#endif // TACHOMETER_H
