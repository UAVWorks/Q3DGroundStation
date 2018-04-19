#include "warning.h"
#include "ui_warning.h"

Warning::Warning(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Warning)
{
  ui->setupUi(this);

  ui->gyro_3d->setText(QObject::tr("3D_GYRO"));
  ui->accel_3d->setText(QObject::tr("3D_ACCEL"));
  ui->mag_3d->setText(QObject::tr("3D_MAG"));
  ui->abs_pressure->setText(QObject::tr("ABS_PRE"));
  ui->diff_pressure->setText(QObject::tr("DIFF_PRE"));
  ui->gps->setText(QObject::tr("GPS"));
  ui->optical_flow->setText(QObject::tr("OPT_FLOW"));
  ui->vision_position->setText(QObject::tr("VIS_POS"));
  ui->laser_position->setText(QObject::tr("LAS_POS"));
  ui->ext_ground_truth->setText(QObject::tr("EXT_GT"));
  ui->arc->setText(QObject::tr("ANG_RATE_CTL"));
  ui->as->setText(QObject::tr("ATT_STA"));
  ui->yaw_pos->setText(QObject::tr("YAW_POS"));
  ui->z_alt_ctl->setText(QObject::tr("Z_ALT_CTL"));
  ui->xy_pos_ctl->setText(QObject::tr("XY_POS_CTL"));
  ui->motor_output->setText(QObject::tr("MOTOR_OUT"));
  ui->rc_rec->setText(QObject::tr("RC_REC"));
  ui->gyro_3d2->setText(QObject::tr("3D_GYRO2"));
  ui->accel_3d2->setText(QObject::tr("3D_ACCEL2"));
  ui->mag_3d2->setText(QObject::tr("3D_MAG2"));
  ui->geofence->setText(QObject::tr("GEOFENCE"));
  ui->ahrs->setText(QObject::tr("AHRS"));
  ui->terrain->setText(QObject::tr("TERRAIN"));
  ui->motor_reversed->setText(QObject::tr("REV_MOTOR"));
  ui->battery->setText(QObject::tr("BATTERY"));
}

Warning::~Warning()
{
  delete ui;
}


/**
 * @brief return warning color by sersor status
 * @param sersor status: present, enabled, health
 * @return
 *      present + enabled + health: return Qt::green
 *      present + enabled + no health: return Qt::yellow
 *      present + disabled: return Qt::transparent
 *      no present: return Qt::red
 *      other: Qt::black
 * @note
 */
QColor Warning::GetItemColor(uint32_t present, uint32_t enabled, uint32_t health, uint32_t status_sensor_type) {
  // 存在 + 使用 + 健康 = green
  if ((present & status_sensor_type) && (enabled & status_sensor_type) && (health & status_sensor_type)) {
    return Qt::green;
  } else if ((present & status_sensor_type) && (enabled & status_sensor_type) && (health ^ status_sensor_type)) {
    // 100000 (&) 100000 = 100000
    // 000000 (^) 100000 = 100000
    // 存在 + 使用 + 不健康 = yellow
    return Qt::yellow;
  } else if ((present & status_sensor_type) && (enabled ^ status_sensor_type)) {
    // 存在 + 不使用 = transparent
    return Qt::transparent;
  } else if ((present ^ status_sensor_type)) {
    // 不存在 = red
    return Qt::red;
  } else {
    // 其他情况显示黑色
    return Qt::black;
  }
}


/**
 * @brief update warn status view
 * @param mavlink_sys_status_t pack reference
 * @return void
 * @note only have 25 warn view, MAV_SYS_STATUS_LOGGING, MAV_SYS_STATUS_SENSOR_ENUM_END no be included !
 *       please see: common.h, line 307 - 309
 */
void Warning::UpdateSysStatusSensor(const mavlink_sys_status_t &sys_status) {
  // MAV_SYS_STATUS_SENSOR_3D_GYRO
  ui->gyro_3d->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_3D_GYRO));
  ui->gyro_3d->UpdateView();

  // MAV_SYS_STATUS_SENSOR_3D_ACCEL
  ui->accel_3d->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_3D_ACCEL));
  ui->accel_3d->UpdateView();

  // MAV_SYS_STATUS_SENSOR_3D_MAG
  ui->mag_3d->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_3D_MAG));
  ui->mag_3d->UpdateView();


  // MAV_SYS_STATUS_SENSOR_ABSOLUTE_PRESSURE
  ui->abs_pressure->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_ABSOLUTE_PRESSURE));
  ui->abs_pressure->UpdateView();


  // MAV_SYS_STATUS_SENSOR_DIFFERENTIAL_PRESSURE
  ui->diff_pressure->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_DIFFERENTIAL_PRESSURE));
  ui->diff_pressure->UpdateView();

  // MAV_SYS_STATUS_SENSOR_GPS
  ui->gps->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_GPS));
  ui->gps->UpdateView();

  // MAV_SYS_STATUS_SENSOR_OPTICAL_FLOW
  ui->optical_flow->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_OPTICAL_FLOW));
  ui->optical_flow->UpdateView();

  // MAV_SYS_STATUS_SENSOR_VISION_POSITION
  ui->vision_position->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_VISION_POSITION));
  ui->vision_position->UpdateView();

  // MAV_SYS_STATUS_SENSOR_LASER_POSITION
  ui->laser_position->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_LASER_POSITION));
  ui->laser_position->UpdateView();

  // MAV_SYS_STATUS_SENSOR_EXTERNAL_GROUND_TRUTH
  ui->ext_ground_truth->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_EXTERNAL_GROUND_TRUTH));
  ui->ext_ground_truth->UpdateView();

  // MAV_SYS_STATUS_SENSOR_ANGULAR_RATE_CONTROL
  ui->arc->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_ANGULAR_RATE_CONTROL));
  ui->arc->UpdateView();

  // MAV_SYS_STATUS_SENSOR_ATTITUDE_STABILIZATION
  ui->as->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_ATTITUDE_STABILIZATION));
  ui->as->UpdateView();

  // MAV_SYS_STATUS_SENSOR_YAW_POSITION
  ui->yaw_pos->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_YAW_POSITION));
  ui->yaw_pos->UpdateView();

  // MAV_SYS_STATUS_SENSOR_Z_ALTITUDE_CONTROL
  ui->z_alt_ctl->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_Z_ALTITUDE_CONTROL));
  ui->z_alt_ctl->UpdateView();

  // MAV_SYS_STATUS_SENSOR_XY_POSITION_CONTROL
  ui->xy_pos_ctl->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_XY_POSITION_CONTROL));
  ui->xy_pos_ctl->UpdateView();


  // MAV_SYS_STATUS_SENSOR_MOTOR_OUTPUTS
  ui->motor_output->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_MOTOR_OUTPUTS));
  ui->motor_output->UpdateView();

  // MAV_SYS_STATUS_SENSOR_RC_RECEIVER
  ui->rc_rec->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_RC_RECEIVER));
  ui->rc_rec->UpdateView();

  // MAV_SYS_STATUS_SENSOR_3D_GYRO2
  ui->gyro_3d2->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_3D_GYRO2));
  ui->gyro_3d2->UpdateView();

  // MAV_SYS_STATUS_SENSOR_3D_ACCEL2
  ui->accel_3d2->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_3D_ACCEL2));
  ui->accel_3d2->UpdateView();

  // MAV_SYS_STATUS_SENSOR_3D_MAG2
  ui->mag_3d2->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_3D_MAG2));
  ui->mag_3d2->UpdateView();

  // MAV_SYS_STATUS_GEOFENCE
  ui->geofence->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_GEOFENCE));
  ui->geofence->UpdateView();

  // MAV_SYS_STATUS_AHRS
  ui->ahrs->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_AHRS));
  ui->ahrs->UpdateView();

  // MAV_SYS_STATUS_TERRAIN
  ui->terrain->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_TERRAIN));
  ui->terrain->UpdateView();

  // MAV_SYS_STATUS_REVERSE_MOTOR
  ui->motor_reversed->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_REVERSE_MOTOR));
  ui->motor_reversed->UpdateView();

  // MAV_SYS_STATUS_SENSOR_BATTERY
  ui->battery->setWarnColor(GetItemColor(sys_status.onboard_control_sensors_present,
                                       sys_status.onboard_control_sensors_enabled,
                                       sys_status.onboard_control_sensors_health,
                                       MAV_SYS_STATUS_SENSOR_BATTERY));
  ui->battery->UpdateView();
}
