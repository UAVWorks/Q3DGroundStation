#ifndef UAV_H
#define UAV_H

#include <QWidget>

#include "common/mavlink.h"

namespace Ui {
  class Uav;
}

class Uav : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(QString uav_id READ getUavName WRITE setUavName)
  Q_PROPERTY(QString system_id READ getSystemId WRITE setSystemId)
  Q_PROPERTY(QString component_id READ getComponentId WRITE setComponentId)
  Q_PROPERTY(uint32_t custom_mode READ getCustomMode WRITE setCustomMode)
  Q_PROPERTY(uint8_t base_mode READ getBaseMode WRITE setBaseMode)
  Q_PROPERTY(uint8_t mav_type READ getMavType WRITE setMavType)
  Q_PROPERTY(uint8_t system_status READ getSystemStatus WRITE setSystemStatus)
  Q_PROPERTY(uint8_t autopilot READ getAutopilot WRITE setAutopilot)
  Q_PROPERTY(uint8_t mavlink_version READ getMavlinkVersion WRITE setMavlinkVersion)
  Q_PROPERTY(int8_t  battery_remaining READ getBatteryRemaining WRITE setBatteryRemaining)
  Q_PROPERTY(uint16_t  throttle READ getThrottle WRITE setThrottle)


public:
  explicit Uav(QWidget *parent = 0);
  ~Uav();

public:
  void UpdateUavInfo(const mavlink_heartbeat_t &heartbeat);

private:
  void InitUavImage();

public:
  QString getUavName();
  QString getSystemId();
  QString getComponentId();

  uint32_t getCustomMode();
  uint8_t getBaseMode();
  uint8_t getMavType();
  uint8_t getSystemStatus();
  uint8_t getAutopilot();
  uint8_t getMavlinkVersion();
  int8_t  getBatteryRemaining();
  uint16_t getThrottle();


  void setUavName(QString uav_name);
  void setSystemId(QString system_id);
  void setComponentId(QString system_id);


  void setCustomMode(uint32_t custom_mode);
  void setBaseMode(uint8_t base_mode);
  void setMavType(uint8_t mav_type);
  void setSystemStatus(uint8_t system_status);
  void setAutopilot(uint8_t autopilot);
  void setMavlinkVersion(uint8_t mavlink_version);
  void setBatteryRemaining(int8_t battery_remaining);
  void setThrottle(uint16_t throttle);

private:
  Ui::Uav *ui;
  QString uav_name_;
  QString system_id_;
  QString component_id_;
  uint32_t custom_mode_;
  uint8_t base_mode_;
  uint8_t mav_type_;
  uint8_t system_status_;
  uint8_t autopilot_;
  uint8_t mavlink_version_;
  int8_t battery_remaining_;
  uint16_t throttle_;
};

#endif // UAV_H
