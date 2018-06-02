#ifndef VSI_H
#define VSI_H

#include <QWidget>

namespace Ui {
class Vsi;
}

class Vsi : public QWidget
{

  Q_OBJECT

  public:
    explicit Vsi(QWidget *parent = 0);
    ~Vsi();

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    void TimerUpdate();


  private:
    void DrawStaticBarkGround(QPainter &painter);
    void DrawDynamicPointer(QPainter &painter, QTransform transform);

  public:
    void setvis(double vis_);
    void UpdateView();

  private:
    Ui::Vsi *ui;
    QTimer *timer_;
    double vis_;

 };

#endif // VSI_H
