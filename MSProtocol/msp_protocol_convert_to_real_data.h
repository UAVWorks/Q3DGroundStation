#ifndef MSP_PROTOCOL_CONVERT_TO_REAL_DATA_H
#define MSP_PROTOCOL_CONVERT_TO_REAL_DATA_H

#include "msp_protocol_structs.h"


struct BaseDataDC
{
    virtual ~BaseDataDC() {}
};


/*! @struct
***************************************************************
* 名称 : MspAttitudeDownDC
* 功能 : MspAttitudeDownDC 下行飞行姿态物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-03-23
* 备注 : 需要使用飞行姿态协议处理类来将协议值转换层物理值，再显示到界面上
***************************************************************/
struct MspAttitudeDownDC : public BaseDataDC
{
  MessageHeader message_header;
  double roll = 0.0;
  double pitch = 0.0;
  double yaw = 0.0;
  boost::uint8_t crc = 0;
};



/*! @struct
***************************************************************
* 名称 : MspMotorDownDC
* 功能 : MspMotorDownDC 下行电机物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-034-04
* 备注 :
***************************************************************/
struct MspMotorDownDC : public BaseDataDC
{
  MessageHeader message_header;   // msg header
  double lt_motor = 0.0;          // left top motor
  double rt_motor = 0.0;          // right top motor
  double lb_motor = 0.0;          // left bottom motor
  double rb_motor = 0.0;          // right bottom motor
  double unuse_one = 0.0;         // don't use
  double unuse_two = 0.0;         // don't use
  double unuse_three = 0.0;       // don't use
  double unuse_four = 0.0;        // don't use
  boost::uint8_t crc   = 0;       // crc
};


/*! @struct
*******************************************************
* 名称 : MspRcDownDC
* 功能 : MSP_RC 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 105, direction: FC->GCS
        All data range [1000 : 2000]
******************************************************/
struct MspRcDownDC : public BaseDataDC
{
  MessageHeader message_header;    // msg header
  double roll = 0.0;
  double pitch = 0.0;
  double yaw = 0.0;
  double throttle = 0.0;
  double aux1 = 0.0;
  double aux2 = 0.0;
  double aux3 = 0.0;
  double aux4 = 0.0;
  boost::uint8_t crc   = 0;
};

/*! @struct
*******************************************************
* 名称 : MspAltitudeDownDC
* 功能 : MSP_ALTITUDE 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 109, direction: FC->GCS
******************************************************/
struct MspAltitudeDownDC : public BaseDataDC
{
  MessageHeader message_header;     // msg header
  double est_alt = 0.0;             // cm
  double vario = 0.0;               // cm/s
  boost::uint8_t crc   = 0;         // crc
};

/*! @struct
*******************************************************
* 名称 : MspAnalogDownDC
* 功能 : MSP_ANALOG 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 110, direction: FC->GCS
******************************************************/
struct MspAnalogDownDC : public BaseDataDC
{
  MessageHeader message_header;    // msg header
  double vbat = 0.0;               // unit: 1/10 volt
  double int_power_meter_sum = 0.0;
  double rssi = 0.0;               // range: [0:1023]
  double amperage = 0.0;
  boost::uint8_t crc   = 0;        // crc
};



/*! @struct
*******************************************************
* 名称 : MspRcTuningDownDC
* 功能 : MSP_RC_TUNING 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 111, direction: FC->GCS
        All data range [0 : 100]
******************************************************/
struct MspRcTuningDownDC : public BaseDataDC
{
  MessageHeader message_header;       // msg header
  double byte_rc_rate = 0.0;          // range [0 : 100]
  double byte_rc_expo = 0.0;          // range [0 : 100]
  double byte_roll_pitch_rate = 0.0;  // range [0 : 100]
  double byte_yaw_rate = 0.0;         // range [0 : 100]
  double byte_dynthr_pid = 0.0;       // range [0 : 100]
  double byte_throttle_mid = 0.0;     // range [0 : 100]
  double byte_throttle_expo = 0.0;    // range [0 : 100]
  boost::uint8_t crc   = 0;           // crc
};


/*! @struct
*******************************************************
* 名称 : MspPIDDownDC
* 功能 : MSP_PID 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 112, direction: FC->GCS
******************************************************/
struct MspPIDDownDC : public BaseDataDC
{
  MessageHeader message_header;   // msg header
  double  roll[3];
  double pitch[3];
  double   yaw[3];
  double   alt[3];
  double   pos[3];
  double  posr[3];
  double  navr[3];
  double level[3];
  double   mag[3];
  double   vel[3];               // vel is not used
  boost::uint8_t  crc = 0;       // crc
};


/*! @struct
*******************************************************
* 名称 : MspMiscDownDC
* 功能 : MSP_MISC 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 114, direction: FC->GCS
******************************************************/
struct MspMiscDownDC : public BaseDataDC
{
  MessageHeader message_header;        // msg header
  double int_power_trigger1 = 0.0;
  double conf_minth_rottle = 0.0;      // minimum throttle to run motor in idle state ( range [1000;2000] )
  double max_throttle = 0.0;           // maximum throttle ( range [1000;2000] )
  double min_command = 0.0;            // throttle at the lowest position ( range [1000;2000] , could be occasionally a little bit less than 1000 depending on ESCs)
  double conf_failsafe_throttle = 0.0; // should be set less than hover state ( range [1000;2000] )
  double plog_arm = 0.0;               // counter
  double plog_lifetime = 0.0;
  double conf_mag_declination = 0.0;   // magnetic declination ( unit:1/10 degree )
  double conf_vbatscale = 0.0;
  double conf_vbatscale_warn1 = 0.0;   // unit: 1/10 volt
  double conf_vbatscale_warn2 = 0.0;   // unit: 1/10 volt
  double conf_vbatscale_crit = 0.0;    // unit: 1/10 volt
  boost::uint8_t crc = 0;              // crc
};



/*! @struct
*******************************************************
* 名称 : MspMotorPinsDownDC
* 功能 : MSP_MOTOR_PINS 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 115, direction: FC->GCS
******************************************************/
struct MspMotorPinsDownDC : public BaseDataDC
{
  MessageHeader message_header;
  double pin_1 = 0.0;
  double pin_2 = 0.0;
  double pin_3 = 0.0;
  double pin_4 = 0.0;
  double pin_5 = 0.0;
  double pin_6 = 0.0;
  double pin_7 = 0.0;
  double pin_8 = 0.0;
  boost::uint8_t crc   = 0;   // crc
};


/*! @struct
*******************************************************
* 名称 : MspBoxNamesDownDC
* 功能 : MSP_BOXNAMES 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 116, direction: FC->GCS
******************************************************/
struct MspBoxNamesDownDC : public BaseDataDC
{
  MessageHeader message_header;
  std::string box_names;
  boost::uint8_t crc   = 0;   // crc
};


/*! @struct
*******************************************************
* 名称 : MspPIDNamesDownDC
* 功能 : MSP_PIDNAMES 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 117, direction: FC->GCS
******************************************************/
struct MspPIDNamesDownDC : public BaseDataDC
{
  MessageHeader message_header;
  std::string pid_names;
  boost::uint8_t crc   = 0;   // crc
};


/*! @struct
*******************************************************
* 名称 : MspBoxIDsDownDC
* 功能 : MSP_BOXIDS 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 119, direction: FC->GCS
        暂时没有定义数据
******************************************************/
struct MspBoxIDsDownDC : public BaseDataDC
{
  MessageHeader message_header;

  boost::uint8_t crc   = 0;   // crc
};



/*! @struct
*******************************************************
* 名称 : MspServoConfDown
* 功能 : MSP_SERVO_CONF 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 120, direction: FC->GCS
        暂时没有定义数据
******************************************************/
struct MspServoConfDownDC : public BaseDataDC
{
  MessageHeader message_header;

  boost::uint8_t crc   = 0;   // crc
};

/*! @struct
*******************************************************
* 名称 : MspRcDeadbandsDown
* 功能 : MSP_RC_Deadbands 下行物理数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 125, direction: FC->GCS
******************************************************/
struct MspRcDeadbandsDownDC : public BaseDataDC
{
  MessageHeader message_header;
  double yaw_deadband = 0.0;
  double alt_deadband = 0.0;
  double pitch_deadband = 0.0;
  double roll_deadband = 0.0;
  boost::uint8_t crc   = 0;   // crc
};





#endif // MSP_PROTOCOL_CONVERT_TO_REAL_DATA_H
