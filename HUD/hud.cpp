#include "hud.h"
#include "ui_hud.h"

HUD::HUD(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::HUD)
{
  ui->setupUi(this);
  //ui->hud->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  //ui->hud->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

HUD::~HUD()
{
  // 程序退出可能出现异常
  delete ui;
}

/**
 * @brief Update HUD
 * @param Attitude pack
 * @note This slot fun connect Connection::UpdateAttitude signal
 */
void HUD::UpdateHUD(const mavlink_attitude_t &attitude) {
  ui->hud->setRoll(attitude.roll);
  ui->hud->setPitch(attitude.pitch);
  ui->hud->setYaw(attitude.yaw);
  ui->hud->update();
}

