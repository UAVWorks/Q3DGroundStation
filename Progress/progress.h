#ifndef PROGRESS_H
#define PROGRESS_H

#include <QWidget>

namespace Ui {
  class Progress;
}

class Progress : public QWidget
{
  Q_OBJECT

public:
  explicit Progress(QWidget *parent = 0);
  ~Progress();

private:
  Ui::Progress *ui;
};

#endif // PROGRESS_H
