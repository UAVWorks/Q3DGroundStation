#ifndef TRACK_PLANNING_H
#define TRACK_PLANNING_H

#include <QDialog>

namespace Ui {
  class TrackPlanning;
}

class TrackPlanning : public QDialog
{
  Q_OBJECT

public:
  explicit TrackPlanning(QWidget *parent = 0);
  ~TrackPlanning();

private:
  Ui::TrackPlanning *ui;
};

#endif // TRACK_PLANNING_H
