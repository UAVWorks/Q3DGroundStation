#ifndef WARN_ITEM_H
#define WARN_ITEM_H

#include <QWidget>

namespace Ui {
  class WarnItem;
}

class WarnItem : public QWidget
{
  Q_OBJECT

public:
  explicit WarnItem(QWidget *parent = 0);
  ~WarnItem();

private:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);

public:
  void setText(QString text);
  void setWarnColor(QColor brush_color);
  void setFontSize(int font_size);
  void UpdateView();

private:
  Ui::WarnItem *ui;
  QColor brush_color_;
  QString text_;
  int font_size_;

};

#endif // WARN_ITEM_H
