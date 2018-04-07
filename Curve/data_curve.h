#ifndef DATA_CURVE_H
#define DATA_CURVE_H

#include <QWidget>

namespace Ui {
class DataCurve;
}

class DataCurve : public QWidget
{
  Q_OBJECT

public:
  explicit DataCurve(QWidget *parent = 0);
  ~DataCurve();

private:
  Ui::DataCurve *ui;
};

#endif // DATA_CURVE_H
