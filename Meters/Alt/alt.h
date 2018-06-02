#ifndef ALT_H
#define ALT_H

#include <QWidget>

namespace Ui {
class Alt;
}

class Alt : public QWidget
{

    Q_OBJECT

public:
  explicit Alt(QWidget *parent = 0);
    ~Alt();

protected:
  void paintEvent(QPaintEvent *event);

private:
  void TimerUpdate();


private:
  void DrawStaticBarkGround(QPainter &painter);
  void DrawDynamicPointer(QPainter &painter, QTransform transform);

public:
  void setAlt(double ALT_);
  void UpdateView();

private:
  Ui::Alt *ui;
  QTimer *timer_ ;
  QString show_text_;
  QString title_ =  "Alt";
  double ALT_ ;
};

#endif // ALT_H
