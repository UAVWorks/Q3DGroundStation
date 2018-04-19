#ifndef COMPASS_WIDGET_H
#define COMPASS_WIDGET_H

#include <QWidget>

class QTimer;

namespace Ui {
  class CompassWidget;
}

class CompassWidget : public QWidget
{
  Q_OBJECT

public:
  explicit CompassWidget(QWidget *parent = 0);
  ~CompassWidget();

protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);

private:
  void DrawStaticBarkGround(QPainter &painter);
public:
  void setAngle(double angle);

private:
  Ui::CompassWidget *ui;
  QTimer *timer_;
  double angle_;
  bool bg_need_redraw_;
  QPixmap temp_pix_;
};

#endif // COMPASS_WIDGET_H
