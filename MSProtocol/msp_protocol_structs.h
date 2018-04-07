#ifndef MSP_PROTOCOL_STRUCTS_H
#define MSP_PROTOCOL_STRUCTS_H
/***************************************************************
* Copyright (c) 2018, 陈登龙
* All rights reserved.
*
* 文件名称：msp_protocol_structs.h
* 摘   要：为本项目定义符合 MSP 通信协议结构体
*
* 当前版本：1.0
* 作   者：陈登龙
* 完成日期：2018-03-10
***************************************************************/


#include <string>
#include <boost/cstdint.hpp>

#include "msp_protocol_command.h"


// 消息头前两个固定字符
const boost::uint8_t kHeaderStart = '$';
const boost::uint8_t kHeaderM = 'M';

// 数据包传输方向标识
const boost::uint8_t kUavToGcs = '>';
const boost::uint8_t kGcsToUav = '<';

/*! @struct
***************************************************************
* 名称 : BaseData
* 功能 : BaseData 是为协议提供基类的结构体。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-03-10
***************************************************************/
struct BaseData
{
    virtual ~BaseData() {}
};




/*! @struct
***************************************************************
* 名称 : MessageHeader
* 功能 : MessageHeader 是为协议提供基类的结构体。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-03-10
***************************************************************/
//#pragma pack(1)
struct MessageHeader
{
    boost::uint8_t header_start = 0;   // MSP 规定的第一个字符
    boost::uint8_t header_m     = 0;   // MSP 规定的第二个字符
    boost::uint8_t direction    = 0;   // 数据包传输方向，'<' 表示发送数据包到飞控，'>' 表示从飞控接收数据包
    boost::uint8_t size         = 0;   // 数据包长度
    boost::uint8_t command      = 0;   // MSP 定义中的消息 message_id
};
//#pragma pack()


/*! @struct
***************************************************************
* 名称 : MSP_IDENT
* 功能 : MSP_IDENT 数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-03-10
* 备注 : message_id = 100, direction: FC->3DGC
***************************************************************/
struct MspIdent : public BaseData
{
    MessageHeader message_header;     // 消息头
    boost::uint8_t version     = 0;   // MultiWii 版本号
    boost::uint8_t multitype   = 0;   // 暂时未知
    boost::uint8_t msp_version = 0;   // 不经常使用
    boost::uint32_t capability = 0;   // 指示飞控板容量的 32 位变量，目前第一位使用 BIND 按钮，第二位使用 DYNBAL，第三位使用 FLAP
    boost::uint8_t crc         = 0;   // crc 校验和
};


/*! @struct
***************************************************************
* 名称 : MSP_STATUS
* 功能 : MSP_STATUS 数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-03-10
* 备注 : message_id = 101, direction: FC->3DGC
***************************************************************/
struct MspStatus : public BaseData
{
    MessageHeader message_header;               // 消息头
    boost::uint16_t cycle_time = 0;             // 单位 ms
    boost::uint16_t i2c_errors_count = 0;       // 无
    boost::uint16_t sensor = 0;                 // BARO << 1 | MAG << 2 | GPS << 3 | SONAR << 4
    boost::uint32_t flag = 0;                   // 用于指示哪个 BOX 处于活动状态的位变量，位的位置取决于配置的 BOX
    boost::uint8_t global_conf_current_set = 0; // 指示当前的配置设置
    boost::uint8_t crc          = 0;   // crc 校验和
};

/*! @struct
***************************************************************
* 名称 : MSP_RAW_IMU
* 功能 : MSP_RAW_IMU 数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-03-10
* 备注 : message_id = 102, direction: FC->3DGC
***************************************************************/
struct MspRawImu : public BaseData
{
    MessageHeader message_header;   // 消息头
    boost::uint16_t accx = 0;
    boost::uint16_t accy = 0;
    boost::uint16_t gyrx = 0;       // 单位依赖于具体的 GYRO 传感器，对于 MPU6050，1 单位 = 1 / 1.096 deg/s
    boost::uint16_t gyry = 0;
    boost::uint16_t gyrz = 0;
    boost::uint16_t magx = 0;       // 单位依赖于具体的 MAG 传感器
    boost::uint16_t magy = 0;
    boost::uint16_t magz = 0;
    boost::uint8_t crc   = 0;       // crc 校验和
};


/*! @struct
***************************************************************
* 名称 : MSP_RAW_IMU
* 功能 : MSP_RAW_IMU 数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-03-10
* 备注 : message_id = 201, direction: FC->3DGC
***************************************************************/
struct MspRawGps : public BaseData
{
    MessageHeader message_header;           // 消息头
    boost::uint8_t gps_fix = 0;             // 0 or 1
    boost::uint8_t gps_num_sat = 0;
    boost::uint32_t gps_coord_lat = 0;      // 1 / 10 000 000 deg
    boost::uint32_t gps_coord_lon = 0;      // 1 / 10 000 000 deg
    boost::uint32_t gps_altitude = 0;       // meter
    boost::uint16_t gps_speed = 0;          // cm/s
    boost::uint16_t gps_ground_course = 0;  // 单位: degree * 10
    boost::uint8_t crc          = 0;   // crc 校验和
};



/*! @struct
***************************************************************
* 名称 : MSP_SET_RAW_IMU
* 功能 : MSP_SET_RAW_IMU 数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-03-10
* 备注 : message_id = 201, direction: 3DGC->FC
***************************************************************/
struct MspSetRawGps : public BaseData
{
    MessageHeader message_header;           // 消息头
    boost::uint8_t gps_fix = 0;             // 0 or 1
    boost::uint8_t gps_num_sat = 0;
    boost::uint32_t gps_coord_lat = 0;      // 1 / 10 000 000 deg
    boost::uint32_t gps_coord_lon = 0;      // 1 / 10 000 000 deg
    boost::uint32_t gps_altitude = 0;       // meter
    boost::uint16_t gps_speed = 0;          // cm/s
    boost::uint16_t gps_ground_course = 0;  // 单位: degree * 10
    boost::uint8_t crc          = 0;   // crc 校验和
};



/*! @struct
***************************************************************
* 名称 : MSP_ATTITUDE
* 功能 : MSP_ATTITUDE 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-03-10
* 备注 : message_id = 108, direction: FC->GCS
        飞控处理对应的上行命令后会返回该姿态数据包
***************************************************************/
struct MspAttitudeDown : public BaseData
{
    MessageHeader message_header;   // 消息头
    boost::int16_t roll  = 0;       // 横滚角
    boost::int16_t pitch = 0;       // 俯仰角
    boost::int16_t yaw   = 0;       // 偏航角
    boost::uint8_t crc   = 0;       // crc 校验和
};

/*! @struct
***************************************************************
* 名称 : MSP_ATTITUDE
* 功能 : MSP_ATTITUDE 上行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-03-10
* 备注 : message_id = 108, direction: GCS->FC
        上行数据包不包含数据，只发命令，飞控处理命令后会返回姿态数据包

        不要 public 继承 BaseData，否则结构体大小会改变！！！
***************************************************************/
//#pragma pack(1)
struct MspAttitudeUp
{
    MessageHeader message_header;   // 消息头
    boost::uint8_t crc   = 0;       // crc 校验和
};
//#pragma pack()


/*! @struct
*******************************************************
* 名称 : MspMotorDown
* 功能 : MSP_MOTOR 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 104, direction: FC->GCS
        飞控处理对应的上行命令后会返回该姿态数据包
******************************************************/
struct MspMotorDown : public BaseData
{
  MessageHeader message_header;       // msg header
  boost::uint16_t lt_motor = 0;       // left top motor
  boost::uint16_t rt_motor = 0;       // right top motor
  boost::uint16_t lb_motor = 0;       // left bottom motor
  boost::uint16_t rb_motor = 0;       // right bottom motor
  boost::uint16_t unuse_one = 0;      // don't use
  boost::uint16_t unuse_two = 0;      // don't use
  boost::uint16_t unuse_three = 0;    // don't use
  boost::uint16_t unuse_four = 0;     // don't use
  boost::uint8_t crc   = 0;           // crc
};

/*! @struct
*******************************************************
* 名称 : MspRcDown
* 功能 : MSP_RC 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 105, direction: FC->GCS
        All data range [1000 : 2000]
******************************************************/
struct MspRcDown : public BaseData
{
  MessageHeader message_header;    // msg header
  boost::uint16_t roll = 0;
  boost::uint16_t pitch = 0;
  boost::uint16_t yaw = 0;
  boost::uint16_t throttle = 0;
  boost::uint16_t aux1 = 0;
  boost::uint16_t aux2 = 0;
  boost::uint16_t aux3 = 0;
  boost::uint16_t aux4 = 0;
  boost::uint8_t crc   = 0;
};

/*! @struct
*******************************************************
* 名称 : MspAltitudeDown
* 功能 : MSP_ALTITUDE 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 109, direction: FC->GCS
******************************************************/
struct MspAltitudeDown : public BaseData
{
  MessageHeader message_header;    // msg header
  boost::int32_t est_alt = 0;      // cm
  boost::int16_t vario = 0;        // cm/s
  boost::uint8_t crc   = 0;        // crc
};

/*! @struct
*******************************************************
* 名称 : MspAnalogDown
* 功能 : MSP_ANALOG 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 110, direction: FC->GCS
******************************************************/
struct MspAnalogDown : public BaseData
{
  MessageHeader message_header;    // msg header
  boost::uint8_t vbat = 0;         // unit: 1/10 volt
  boost::uint16_t int_power_meter_sum = 0;
  boost::uint16_t rssi = 0;        // range: [0:1023]
  boost::uint16_t amperage = 0;
  boost::uint8_t crc   = 0;        // crc
};



/*! @struct
*******************************************************
* 名称 : MspRcTuningDown
* 功能 : MSP_RC_TUNING 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 111, direction: FC->GCS
        All data range [0 : 100]
******************************************************/
struct MspRcTuningDown : public BaseData
{
  MessageHeader message_header;             // msg header
  boost::uint8_t byte_rc_rate = 0;          // range [0 : 100]
  boost::uint8_t byte_rc_expo = 0;          // range [0 : 100]
  boost::uint8_t byte_roll_pitch_rate = 0;  // range [0 : 100]
  boost::uint8_t byte_yaw_rate = 0;         // range [0 : 100]
  boost::uint8_t byte_dynthr_pid = 0;       // range [0 : 100]
  boost::uint8_t byte_throttle_mid = 0;     // range [0 : 100]
  boost::uint8_t byte_throttle_expo = 0;    // range [0 : 100]
  boost::uint8_t crc   = 0;                 // crc
};


/*! @struct
*******************************************************
* 名称 : MspPIDDown
* 功能 : MSP_PID 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 112, direction: FC->GCS
******************************************************/
struct MspPIDDown : public BaseData
{
  MessageHeader message_header;   // msg header
  boost::uint8_t  roll[3];
  boost::uint8_t pitch[3];
  boost::uint8_t   yaw[3];
  boost::uint8_t   alt[3];
  boost::uint8_t   pos[3];
  boost::uint8_t  posr[3];
  boost::uint8_t  navr[3];
  boost::uint8_t level[3];
  boost::uint8_t   mag[3];
  boost::uint8_t   vel[3];       // vel is not used
  boost::uint8_t  crc = 0;       // crc
};


/*! @struct
*******************************************************
* 名称 : MspMiscDown
* 功能 : MSP_MISC 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 114, direction: FC->GCS
******************************************************/
struct MspMiscDown : public BaseData
{
  MessageHeader message_header;               // msg header
  boost::uint16_t int_power_trigger1 = 0;
  boost::uint16_t conf_minth_rottle = 0;      // minimum throttle to run motor in idle state ( range [1000;2000] )
  boost::uint16_t max_throttle = 0;           // maximum throttle ( range [1000;2000] )
  boost::uint16_t min_command = 0;            // throttle at the lowest position ( range [1000;2000] , could be occasionally a little bit less than 1000 depending on ESCs)
  boost::uint16_t conf_failsafe_throttle = 0; // should be set less than hover state ( range [1000;2000] )
  boost::uint16_t plog_arm = 0;               // counter
  boost::uint32_t plog_lifetime = 0;
  boost::uint16_t conf_mag_declination = 0;   // magnetic declination ( unit:1/10 degree )
  boost::uint8_t conf_vbatscale = 0;
  boost::uint8_t conf_vbatscale_warn1 = 0;    // unit: 1/10 volt
  boost::uint8_t conf_vbatscale_warn2 = 0;    // unit: 1/10 volt
  boost::uint8_t conf_vbatscale_crit = 0;     // unit: 1/10 volt
  boost::uint8_t crc = 0;                     // crc
};



/*! @struct
*******************************************************
* 名称 : MspMotorPinsDown
* 功能 : MSP_MOTOR_PINS 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 115, direction: FC->GCS
******************************************************/
struct MspMotorPinsDown : public BaseData
{
  MessageHeader message_header;
  boost::uint8_t pin_1 = 0;
  boost::uint8_t pin_2 = 0;
  boost::uint8_t pin_3 = 0;
  boost::uint8_t pin_4 = 0;
  boost::uint8_t pin_5 = 0;
  boost::uint8_t pin_6 = 0;
  boost::uint8_t pin_7 = 0;
  boost::uint8_t pin_8 = 0;
  boost::uint8_t crc   = 0;   // crc
};


/*! @struct
*******************************************************
* 名称 : MspBoxNamesDown
* 功能 : MSP_BOXNAMES 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 116, direction: FC->GCS
******************************************************/
struct MspBoxNamesDown : public BaseData
{
  MessageHeader message_header;
  std::string box_names;
  boost::uint8_t crc   = 0;   // crc
};


/*! @struct
*******************************************************
* 名称 : MspPIDNamesDown
* 功能 : MSP_PIDNAMES 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 117, direction: FC->GCS
******************************************************/
struct MspPIDNamesDown : public BaseData
{
  MessageHeader message_header;
  std::string pid_names;
  boost::uint8_t crc   = 0;   // crc
};


/*! @struct
*******************************************************
* 名称 : MspBoxIDsDown
* 功能 : MSP_BOXIDS 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 119, direction: FC->GCS
        暂时没有定义数据
******************************************************/
struct MspBoxIDsDown : public BaseData
{
  MessageHeader message_header;

  boost::uint8_t crc   = 0;   // crc
};



/*! @struct
*******************************************************
* 名称 : MspServoConfDown
* 功能 : MSP_SERVO_CONF 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 120, direction: FC->GCS
        暂时没有定义数据
******************************************************/
struct MspServoConfDown : public BaseData
{
  MessageHeader message_header;

  boost::uint8_t crc   = 0;   // crc
};

/*! @struct
*******************************************************
* 名称 : MspRcDeadbandsDown
* 功能 : MSP_RC_Deadbands 下行数据包定义。
* 作者 : 陈登龙 南昌航空大学信息工程学院自动控制系。
* 时间 : 2018-04-04
* 备注 : message_id = 125, direction: FC->GCS
******************************************************/
struct MspRcDeadbandsDown : public BaseData
{
  MessageHeader message_header;
  boost::uint8_t yaw_deadband = 0;
  boost::uint8_t alt_deadband = 0;
  boost::uint8_t pitch_deadband = 0;
  boost::uint8_t roll_deadband = 0;
  boost::uint8_t crc   = 0;   // crc
};




#endif // MSP_PROTOCOL_STRUCTS_H
