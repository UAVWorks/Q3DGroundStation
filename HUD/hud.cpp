#include "hud.h"
#include "ui_hud.h"

#include "../MSProtocol/msp_protocol_convert_to_real_data.h"


HUD::HUD(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::HUD)
{
  ui->setupUi(this);
}

HUD::~HUD()
{
  delete ui;
}


void HUD::UpdateHUD(const MspAttitudeDownDC &maddc) {
  ui->hud->setRoll(maddc.roll);
  ui->hud->setPitch(maddc.pitch);
  ui->hud->update();
}
