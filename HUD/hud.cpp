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
 * @note This slot fun connect Connection::UpdateHUD signal
 */
void HUD::UpdateHUD(const mavlink_vfr_hud_t &hud) {
  ui->hud->setSpeed(hud.airspeed);
  ui->hud->setGroundSpeed(hud.groundspeed);
  ui->hud->setAltitude(hud.alt);
  ui->hud->setClimb(hud.climb);
  ui->hud->setHeading(hud.heading);
  ui->hud->setThrottle(hud.throttle);
  ui->hud->update();
}

/**
 * @brief Update Attitude
 * @param Attitude pack
 * @note This slot fun connect Connection::UpdateAttitude signal
 */
void HUD::UpdateAttitude(const mavlink_attitude_t &attitude) {
  ui->hud->setRoll(attitude.roll);
  ui->hud->setPitch(attitude.pitch);
  ui->hud->setYaw(attitude.yaw);
  ui->hud->update();
}

/**
 * @brief Update GPS
 * @param GPS pack
 * @note This slot fun connect Connection::UpdateGPSStatus signal
 */
void HUD::UpdateGPSStatus(const bool gps_status) {
  ui->hud->setGPS(gps_status);
  ui->hud->update();
}

void HUD::UpdateBatteryRemain(const int8_t battery_remaining) {
  ui->hud->setBattery(battery_remaining);
  ui->hud->update();
}

