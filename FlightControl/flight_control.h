#ifndef FLIGHT_CONTROL_H
#define FLIGHT_CONTROL_H

#include <QWidget>

namespace Ui {
  class FlightControl;
}

class FlightControl : public QWidget
{
  Q_OBJECT

public:
  explicit FlightControl(QWidget *parent = 0);
  ~FlightControl();
protected:
  void paintEvent(QPaintEvent *event);
private:
  Ui::FlightControl *ui;
};

#endif // FLIGHT_CONTROL_H
