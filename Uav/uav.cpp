#include "uav.h"
#include "ui_uav.h"

#include "common/mavlink.h"

Uav::Uav(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Uav)
{
  ui->setupUi(this);
  InitUavImage();
}

Uav::~Uav()
{
  delete ui;
}

void Uav::UpdateUavInfo(const mavlink_heartbeat_t &heartbeat) {
  setUavName(QString("MAV"));
  setSystemId(QString("1"));
  setComponentId(QString("1"));
  setCustomMode(heartbeat.custom_mode);
  setBaseMode(heartbeat.base_mode);
  setMavType(heartbeat.type);
  setSystemStatus(heartbeat.system_status);
  setAutopilot(heartbeat.autopilot);
  setMavlinkVersion(heartbeat.mavlink_version);
}

void Uav::InitUavImage() {
  //ui->uav_image->setPixmap(QPixmap("E:/Qt Project/Graduation Project/Q3DGroundStation/uav.png"));
}

QString Uav::getUavName() {
  return uav_name_;
}


void Uav::setUavName(QString uav_name) {
  //ui->uav_name->setText(uav_name);
}


QString Uav::getSystemId() {
  return system_id_;
}

void Uav::setSystemId(QString system_id) {
  ui->sys_id->setText(system_id);
}

QString Uav::getComponentId() {
  return component_id_;
}

void Uav::setComponentId(QString component_id) {
  ui->compon_id->setText(component_id);
}


uint32_t Uav::getCustomMode() {
  return custom_mode_;
}

void Uav::setCustomMode(uint32_t custom_mode) {
  custom_mode_ = custom_mode;

  // what is custom_mode in APM ???
  switch (custom_mode_ & 0xFF) {
    case 0:
      ui->custom_mode->setText(QString("STABILIZE"));
      break;
    case 1:
      ui->custom_mode->setText(QString("ACRO"));
      break;
    case 2:
      ui->custom_mode->setText(QString("ALT_HOLD"));
      break;
    case 3:
      ui->custom_mode->setText(QString("AUTO"));
      break;
    case 4:
      // 引导模式
      ui->custom_mode->setText(QString("GUIDED"));
      break;
    case 5:
      ui->custom_mode->setText(QString("LOITER"));
      break;
    case 6:
      ui->custom_mode->setText(QString("RTL"));
      break;
    case 7:
      ui->custom_mode->setText(QString("CIRCLE"));
      break;
    case 9:
      ui->custom_mode->setText(QString("LAND"));
      break;
    case 11:
      ui->custom_mode->setText(QString("DRIFT"));
      break;
    case 13:
      ui->custom_mode->setText(QString("SPORT"));
      break;
    case 14:
      ui->custom_mode->setText(QString("FLIP"));
      break;
    case 15:
      ui->custom_mode->setText(QString("AUTOTUNE"));
      break;
    case 16:
      ui->custom_mode->setText(QString("POSHOLD"));
      break;
    case 17:
      ui->custom_mode->setText(QString("BRAKE"));
      break;
    case 18:
      ui->custom_mode->setText(QString("THROW"));
      break;
    case 19:
      ui->custom_mode->setText(QString("AVOID_ADSB"));
      break;
    case 20:
      ui->custom_mode->setText(QString("GUIDED_NOGPS"));
      break;
    case 21:
      ui->custom_mode->setText(QString("SMART_RTL"));
      break;
    default:
      break;
  }
}


uint8_t Uav::getBaseMode() {
  return base_mode_;
}


void Uav::setBaseMode(uint8_t base_mode) {
  base_mode_ = base_mode;
  QString mode = "";

  if (base_mode_ & MAV_MODE_FLAG_CUSTOM_MODE_ENABLED) {
    mode = QString("C|");
  }

  if (base_mode_ & MAV_MODE_FLAG_TEST_ENABLED) {
    mode += QString("T|");
  }

  if (base_mode_ & MAV_MODE_FLAG_AUTO_ENABLED) {
    mode += QString("A|");
  }

  // 引导模式
  if (base_mode_ & MAV_MODE_FLAG_GUIDED_ENABLED) {
    mode += QString("G|");
  }

  if (base_mode_ & MAV_MODE_FLAG_STABILIZE_ENABLED) {
    mode += QString("S|");
  }

  if (base_mode_ & MAV_MODE_FLAG_HIL_ENABLED) {
    mode += QString("H|");
  }

  if (base_mode_ & MAV_MODE_FLAG_MANUAL_INPUT_ENABLED) {
    mode += QString("MI|");
  }

  if (base_mode_ & MAV_MODE_FLAG_SAFETY_ARMED) {
    mode += QString("SA|");
  }

  if (base_mode_ & MAV_MODE_FLAG_ENUM_END) {
    mode += QString("E|");
  }

  ui->base_mode->setText(mode);


#if 0
  switch (base_mode_ & 0xFF) {
    case MAV_MODE_FLAG_CUSTOM_MODE_ENABLED:
      ui->base_mode->setText(QString("CUSTOM"));
      break;
    case MAV_MODE_FLAG_TEST_ENABLED:
      ui->base_mode->setText(QString("TEST"));
      break;
    case MAV_MODE_FLAG_AUTO_ENABLED:
      ui->base_mode->setText(QString("AUTO"));
      break;
    case MAV_MODE_FLAG_GUIDED_ENABLED:
      ui->base_mode->setText(QString("GUIDED"));
      break;
    case MAV_MODE_FLAG_STABILIZE_ENABLED:
      ui->base_mode->setText(QString("STABILIZE"));
      break;
    case MAV_MODE_FLAG_HIL_ENABLED:
      ui->base_mode->setText(QString("HIL"));
      break;
    case MAV_MODE_FLAG_MANUAL_INPUT_ENABLED:
      ui->base_mode->setText(QString("MANUAL_INPUT"));
      break;
    case MAV_MODE_FLAG_SAFETY_ARMED:
      ui->base_mode->setText(QString("SAFETY"));
      break;
    default:
      break;
  }
#endif

}

uint8_t Uav::getMavType() {
  return mav_type_;
}

void Uav::setMavType(uint8_t mav_type) {
  mav_type_ = mav_type;
  switch (mav_type_ & 0xFF) {
    case MAV_TYPE_GENERIC:
      ui->mav_type->setText(QString("Generic"));
      break;
    case MAV_TYPE_FIXED_WING:
      ui->mav_type->setText(QString("FixedWing"));
      break;
    case MAV_TYPE_QUADROTOR:
      ui->mav_type->setText(QString("Quadrotor"));
      break;
    case MAV_TYPE_COAXIAL:
      ui->mav_type->setText(QString("Coaxial"));
      break;
    case MAV_TYPE_HELICOPTER:
      ui->mav_type->setText(QString("Helicopter"));
      break;
    case MAV_TYPE_ANTENNA_TRACKER:
      ui->mav_type->setText(QString("AntennaTracker"));
      break;
    case MAV_TYPE_GCS:
      ui->mav_type->setText(QString("GCS"));
      break;
    case MAV_TYPE_AIRSHIP:
      ui->mav_type->setText(QString("Airship"));
      break;
    case MAV_TYPE_FREE_BALLOON:
      ui->mav_type->setText(QString("FreeBalloon"));
      break;
    case MAV_TYPE_ROCKET:
      ui->mav_type->setText(QString("Rocket"));
      break;
    case MAV_TYPE_GROUND_ROVER:
      ui->mav_type->setText(QString("GroundRover"));
      break;
    case MAV_TYPE_SURFACE_BOAT:
      ui->mav_type->setText(QString("SurfaceBoat"));
      break;
    case MAV_TYPE_SUBMARINE:
      ui->mav_type->setText(QString("Submarine"));
      break;
    case MAV_TYPE_HEXAROTOR:
      ui->mav_type->setText(QString("Hexarotor"));
      break;
    case MAV_TYPE_OCTOROTOR:
      ui->mav_type->setText(QString("Octrotor"));
      break;
    case MAV_TYPE_TRICOPTER:
      ui->mav_type->setText(QString("Tricopter"));
      break;
    case MAV_TYPE_FLAPPING_WING:
      ui->mav_type->setText(QString("FlappingWing"));
      break;
    case MAV_TYPE_KITE:
      ui->mav_type->setText(QString("Kite"));
      break;
    case MAV_TYPE_ONBOARD_CONTROLLER:
      ui->mav_type->setText(QString("OnboardController"));
      break;
    case MAV_TYPE_VTOL_DUOROTOR:
      ui->mav_type->setText(QString("VtolDuorotor"));
      break;
    case MAV_TYPE_VTOL_QUADROTOR:
      ui->mav_type->setText(QString("VtolQuadrotor"));
      break;
    case MAV_TYPE_VTOL_TILTROTOR:
      ui->mav_type->setText(QString("VtolTiltrotor"));
      break;
    case MAV_TYPE_VTOL_RESERVED2:
      ui->mav_type->setText(QString("VtolReserved2"));
      break;
    case MAV_TYPE_VTOL_RESERVED3:
      ui->mav_type->setText(QString("VtolReserved3"));
      break;
    case MAV_TYPE_VTOL_RESERVED4:
      ui->mav_type->setText(QString("VtolReserved4"));
      break;
    case MAV_TYPE_VTOL_RESERVED5:
      ui->mav_type->setText(QString("VtolReserved5"));
      break;
    case MAV_TYPE_GIMBAL:
      ui->mav_type->setText(QString("Gimbal"));
      break;
    case MAV_TYPE_ADSB:
      ui->mav_type->setText(QString("Adsb"));
      break;
    case MAV_TYPE_PARAFOIL:
      ui->mav_type->setText(QString("Parafoil"));
      break;
    case MAV_TYPE_DODECAROTOR:
      ui->mav_type->setText(QString("Dodecarotor"));
      break;
    case MAV_TYPE_CAMERA:
      ui->mav_type->setText(QString("Camera"));
      break;
    case MAV_TYPE_CHARGING_STATION:
      ui->mav_type->setText(QString("ChargingStation"));
      break;
    default:
      break;
  }
}

uint8_t Uav::getSystemStatus() {
  return system_status_;
}

void Uav::setSystemStatus(uint8_t system_status) {
  system_status_ = system_status;
  switch (system_status_ & 0xFF) {
    case MAV_STATE_UNINIT:
      ui->system_status->setText(QString("Uninit"));
      break;
    case MAV_STATE_BOOT:
      ui->system_status->setText(QString("Boot"));
      break;
    case MAV_STATE_CALIBRATING:
      ui->system_status->setText(QString("Calibrating"));
      break;
    case MAV_STATE_STANDBY:
      ui->system_status->setText(QString("Standby"));
      break;
    case MAV_STATE_ACTIVE:
      ui->system_status->setText(QString("Active"));
      break;
    case MAV_STATE_CRITICAL:
      ui->system_status->setText(QString("Critical"));
      break;
    case MAV_STATE_EMERGENCY:
      ui->system_status->setText(QString("Emergency"));
      break;
    case MAV_STATE_POWEROFF:
      ui->system_status->setText(QString("Poweroff"));
      break;
    case MAV_STATE_FLIGHT_TERMINATION:
      ui->system_status->setText(QString("FlightTermination"));
      break;
    default:
      break;
  }
}

uint8_t Uav::getAutopilot() {
  return autopilot_;
}

void Uav::setAutopilot(uint8_t autopilot) {
  autopilot_ = autopilot;
  switch (system_status_ & 0xFF) {
    case MAV_AUTOPILOT_GENERIC:
      ui->autopilot->setText(QString("Generic"));
      break;
    case MAV_AUTOPILOT_RESERVED:
      ui->autopilot->setText(QString("Reserved"));
      break;
    case MAV_AUTOPILOT_SLUGS:
      ui->autopilot->setText(QString("Slugs"));
      break;
    case MAV_AUTOPILOT_ARDUPILOTMEGA:
      ui->autopilot->setText(QString("Ardupilotmega"));
      break;
    case MAV_AUTOPILOT_OPENPILOT:
      ui->autopilot->setText(QString("Openpilot"));
      break;
    case MAV_AUTOPILOT_GENERIC_WAYPOINTS_ONLY:
      ui->autopilot->setText(QString("GenericWaypointsOnly"));
      break;
    case MAV_AUTOPILOT_GENERIC_WAYPOINTS_AND_SIMPLE_NAVIGATION_ONLY:
      ui->autopilot->setText(QString("GenericWaypointsAndSimpleNavigationOnly"));
      break;
    case MAV_AUTOPILOT_GENERIC_MISSION_FULL:
      ui->autopilot->setText(QString("GenericMissionFull"));
      break;
    case MAV_AUTOPILOT_INVALID:
      ui->autopilot->setText(QString("Invalid"));
      break;
    case MAV_AUTOPILOT_PPZ:
      ui->autopilot->setText(QString("PPZ"));
      break;
    case MAV_AUTOPILOT_UDB:
      ui->autopilot->setText(QString("UDB"));
      break;
    case MAV_AUTOPILOT_FP:
      ui->autopilot->setText(QString("FP"));
      break;
    case MAV_AUTOPILOT_PX4:
      ui->autopilot->setText(QString("PX4"));
      break;
    case MAV_AUTOPILOT_SMACCMPILOT:
      ui->autopilot->setText(QString("Smaccmpilot"));
      break;
    case MAV_AUTOPILOT_AUTOQUAD:
      ui->autopilot->setText(QString("Autoquad"));
      break;
    case MAV_AUTOPILOT_ARMAZILA:
      ui->autopilot->setText(QString("Armzaila"));
      break;
    case MAV_AUTOPILOT_AEROB:
      ui->autopilot->setText(QString("Aerob"));
      break;
    case MAV_AUTOPILOT_ASLUAV:
      ui->autopilot->setText(QString("Asluav"));
      break;
    case MAV_AUTOPILOT_SMARTAP:
      ui->autopilot->setText(QString("Smartap"));
      break;
    case MAV_AUTOPILOT_AIRRAILS:
      ui->autopilot->setText(QString("Airrails"));
      break;
    default:
      break;
  }
}

uint8_t Uav::getMavlinkVersion() {
  return mavlink_version_;
}

void Uav::setMavlinkVersion(uint8_t mavlink_version) {
  mavlink_version_ = mavlink_version;
  ui->mavlink_version->setText(QString::number(mavlink_version_));
}

int8_t Uav::getBatteryRemaining() {
  return battery_remaining_;
}

void Uav::setBatteryRemaining(int8_t battery_remaining) {
  battery_remaining_ = battery_remaining;
  //ui->battery_bar->setValue(battery_remaining_);
}


uint16_t Uav::getThrottle() {
  return throttle_;
}

void Uav::setThrottle(uint16_t throttle) {
  throttle_ = throttle;
  ui->throttle_bar->setValue(throttle);
}


























