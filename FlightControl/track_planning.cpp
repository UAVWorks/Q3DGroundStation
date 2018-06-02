#include "track_planning.h"
#include "ui_trackplanning.h"



TrackPlanning::TrackPlanning(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::TrackPlanning)
{
  ui->setupUi(this);
}

TrackPlanning::~TrackPlanning()
{
  delete ui;
}
