#include "connection.h"
#include "ui_connection.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QTimer>
#include <QUdpSocket>

#include "q3dgs.h"
#include "Uav/uav.h"
#include "ardupilotmega/mavlink.h"

#include "serial_read_thread.h"
#include "serial_write_thread.h"

#include <QScrollBar>
#include <thread>

#define SERIAL_READ_ALL_ONCE

Connection::Connection(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Connection),
  //tcp_client_(nullptr),
  serial_(new QSerialPort(this)),
  timer_(new QTimer(this)),
  unlock_timer_(new QTimer(this)),
  socket_(new QUdpSocket(this))
{
  ui->setupUi(this);

  //ui->heartbeat_label->setText(QObject::tr("0"));

  // set default ip, port
  ui->udp_ip->setText(QObject::tr("127.0.0.1"));
  ui->udp_port->setText(QObject::tr("10002"));

  // set default serial port(COM4) and bps(115200)
  ui->serialport_select->setCurrentIndex(3);
  ui->bps_select->setCurrentIndex(0);
  //uav_ = new Uav(ui->uav);
  //uav_->show();

  QPalette pal(ui->uav->palette());
  ui->uav->setAutoFillBackground(true);
  pal.setColor(QPalette::Background, Qt::black);
  ui->uav->setPalette(pal);

  socket_->bind(QHostAddress::LocalHost, 10002);
  connect(socket_, SIGNAL(readyRead()),this, SLOT(ReadyRead()));

  //connect(serial_, &QSerialPort::readyRead, this, &Connection::ReadData);
  connect(timer_, &QTimer::timeout, this, &Connection::TimerUpdate);
  connect(unlock_timer_, &QTimer::timeout, this, &Connection::SendUnlockPack);
}

Connection::~Connection()
{
  delete ui;

  if (serial_ != nullptr) {
    if (serial_->isOpen()) {
      serial_->close();
    }
    delete serial_;
  }
}


/**
 * @brief Init serial
 *
 * @note COM3, 115200
 */
void Connection::on_serial_connect_clicked()
{
  if (ui->serial_connect->text() == QObject::tr("disconn")) {
    // close serial
    ui->serial_connect->setText(QObject::tr("connect"));
    timer_->stop();
    serial_->close();
    QMessageBox::information(NULL, "Serial Info", "Serial close success.", QMessageBox::Yes);
  } else {
    // set com
    serial_->setPortName(ui->serialport_select->currentText());
    // set bps
    serial_->setBaudRate(ui->bps_select->currentText().toInt());
    // set data bit = 8 bit
    serial_->setDataBits(QSerialPort::Data8);
    // set parity = no
    serial_->setParity(QSerialPort::NoParity);
    // set flow ctl = no
    serial_->setFlowControl(QSerialPort::NoFlowControl);
    // set stop bit = one
    serial_->setStopBits(QSerialPort::OneStop);
    // open serial port
    if (serial_->open(QIODevice::ReadWrite)) {
      QMessageBox::information(NULL, "Serial Info", "Serial open success.", QMessageBox::Yes);
      ui->serial_connect->setText(QObject::tr("disconn"));

      // start serial read thread
#if 1
      read_control_ = new ReadSerialController(serial_);
      connect(read_control_, &ReadSerialController::UpdateUavInfo, this, &Connection::UpdateUavInfoSlot);
      connect(read_control_, &ReadSerialController::UpdateAttitude, this, &Connection::UpdateAttitudeSlot);
      connect(read_control_, &ReadSerialController::UpdateBatteryStatus, this, &Connection::UpdateBatteryStatusSlot);
      connect(read_control_, &ReadSerialController::UpdateSysStatusSensor, this, &Connection::UpdateSysStatusSensorSlot);
      connect(read_control_, &ReadSerialController::UpdateGlobalPositionInt, this, &Connection::UpdateGlobalPositionIntSlot);
      connect(read_control_, &ReadSerialController::UpdateHUD, this, &Connection::UpdateHUDSlot);
      connect(read_control_, &ReadSerialController::UpdateServoOutput, this, &Connection::UpdateServoOutputSlot);

      // start serial write thread
      //write_control_ = new WriteSerialController(serial_);
      //connect(this, &Connection::WriteMessage, write_control_, &WriteSerialController::WriteMessage);
#endif
      timer_->start(1000);
      RequestDataStream();
    } else {
      QMessageBox::information(NULL, "Serial Info", "Serial open fail.", QMessageBox::Yes);
    }
  }
}

/**
 * @brief Request data stream
 *
 * @note
 */
void Connection::RequestDataStream() {
  // pack msg
  mavlink_message_t msg;
  mavlink_request_data_stream_t request;
  request.target_system = 1;
  request.target_component = 1;
  request.req_stream_id = MAV_DATA_STREAM_ALL;
  request.start_stop = 1;
  // usb: 10
  request.req_message_rate = 10;
  // Telem: 25
  //request.req_message_rate = 25;

  mavlink_msg_request_data_stream_encode(1, 1, &msg, &request);

  // send msg
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  // send to buf
  uint8_t len = mavlink_msg_to_send_buffer(buf, &msg);

  // copy to byte array from buf
  QByteArray out_data;
  out_data.clear();
  for (int i = 0; i < len; i++) {
    out_data.append(buf[i]);
  }

  // I don`t know this function whether to be used
  // QByteArray::fromRawData(buf, len);

  WriteData(out_data);
  //emit WriteMessage(out_data);
}

void Connection::SendHeartbeat() {
  mavlink_heartbeat_t heartbeat;
  //heartbeat.autopilot
}


/**
 * @brief Version Handshaking
 *
 * @note Yes, Can get heartbeat!
 */
void Connection::VersionHandshaking() {
  mavlink_message_t msg;
  mavlink_command_long_t cmd;
  cmd.command = MAV_CMD_REQUEST_PROTOCOL_VERSION;
  cmd.target_system = 1;
  cmd.target_component = 1;
  mavlink_msg_command_long_encode(1, 1, &msg, &cmd);

  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  // send to buf
  uint8_t len = mavlink_msg_to_send_buffer(buf, &msg);

  // copy to byte array from buf
  QByteArray out_data;
  out_data.clear();
  for (int i = 0; i < len; i++) {
    out_data.append(buf[i]);
  }

  // I don`t know this function whether to be used
  // QByteArray::fromRawData(buf, len);

  WriteData(out_data);
}


/**
 * @brief Handle serial data
 *
 * @note
 */
void Connection::HandleMessage(const QByteArray &read_data) {
  mavlink_message_t msg;
  mavlink_status_t status;
  for (int i = 0; i < read_data.length(); i++) {
    if (mavlink_parse_char(MAVLINK_COMM_0, read_data[i], &msg, &status)) {

      /*
      // check if we received version 2 and request a switch.
      if (!(mavlink_get_channel_status(MAVLINK_COMM_0)->flags & MAVLINK_STATUS_FLAG_IN_MAVLINK1)) {
        //this will only switch to proto version 2
        mavlink_get_channel_status(MAVLINK_COMM_0)->flags &= ~(MAVLINK_STATUS_FLAG_OUT_MAVLINK1);
      }
      */

      switch (msg.msgid) {
        /*
        case MAVLINK_MSG_ID_PROTOCOL_VERSION:
          mavlink_protocol_version_t version;
          mavlink_msg_protocol_version_decode(&msg, &version);
          break;
        */
        // Cmd Ack
        case MAVLINK_MSG_ID_COMMAND_ACK:
          mavlink_command_ack_t cmd_ack;
          mavlink_msg_command_ack_decode(&msg, &cmd_ack);
          emit UpdateCmdAck(cmd_ack);
          ui->uav_mag->append("mavlink_command_ack_t");
          break;
        // Heartbeat - ok
        case MAVLINK_MSG_ID_HEARTBEAT:
          mavlink_heartbeat_t heartbeat;
          // decode MAVLink msg
          // Bug: Why Mavlink version is 3 ?
          mavlink_msg_heartbeat_decode(&msg, &heartbeat);

          StopSendUnlockPack(heartbeat.base_mode);
          emit UpdateARM(heartbeat.base_mode);
          emit UpdateMavMode(heartbeat.custom_mode);

          // Update connect uav ui
          ui->uav->UpdateUavInfo(heartbeat);
          ui->uav_mag->append("mavlink_heartbeat_t");
          break;
        // Attitude - ok
        case MAVLINK_MSG_ID_ATTITUDE:
          mavlink_attitude_t attitude;
          mavlink_msg_attitude_decode(&msg, &attitude);
          emit UpdateAttitude(attitude);
          ui->uav_mag->append("mavlink_attitude_t");
          break;
        // Altitude - no use
        /*
        case MAVLINK_MSG_ID_ALTITUDE:
          mavlink_altitude_t altitude;
          mavlink_msg_altitude_decode(&msg, &altitude);
          //emit UpdateAltitude(altitude);
          //ui->uav_mag->append("mavlink_altitude_t");
          break;
        */
        // Battery
        case MAVLINK_MSG_ID_BATTERY_STATUS:
          mavlink_battery_status_t battery;
          mavlink_msg_battery_status_decode(&msg, &battery);

          ui->uav->setBatteryRemaining(battery.battery_remaining);
          ui->uav_mag->append("mavlink_battery_status_t");
          break;
        // Control system state
        /*
        case MAVLINK_MSG_ID_CONTROL_SYSTEM_STATE:
          mavlink_control_system_state_t ctl_sys_state;
          mavlink_msg_control_system_state_decode(&msg, &ctl_sys_state);
          emit UpdateAirSpeed(ctl_sys_state.airspeed);
          ui->uav_mag->append("mavlink_control_system_state_t");
          break;
        // GPS
        case MAVLINK_MSG_ID_GPS_RAW_INT:
          mavlink_gps_raw_int_t gps;
          mavlink_msg_gps_raw_int_decode(&msg, &gps);
          //emit UpdateGPS(gps);
          ui->uav_mag->append("mavlink_gps_raw_int_t");
          break;
        */
        // Sys Status
        case MAVLINK_MSG_ID_SYS_STATUS:
          mavlink_sys_status_t sys_status;
          mavlink_msg_sys_status_decode(&msg, &sys_status);
          emit UpdateSysStatusSensor(sys_status);
          ui->uav_mag->append("mavlink_sys_status_t");
          break;
        // Gps Status
        /*
        case MAVLINK_MSG_ID_GPS_STATUS:
          mavlink_gps_status_t gps_status;
          mavlink_msg_gps_status_decode(&msg, &gps_status);
          // Note: Don't sure satellites_visible -> gps_use
          //emit UpdateGPSStatus(gps_status.satellites_visible);
          ui->uav_mag->append("mavlink_gps_status_t");
          break;
        */
        // Global Position
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
          mavlink_global_position_int_t global_position;
          mavlink_msg_global_position_int_decode(&msg, &global_position);
          emit UpdateGlobalPositionInt(global_position);
          ui->uav_mag->append("mavlink_global_position_int_t");
          break;
        // HUD - ok
        case MAVLINK_MSG_ID_VFR_HUD:
          mavlink_vfr_hud_t vfr_hud;
          mavlink_msg_vfr_hud_decode(&msg, &vfr_hud);
          emit UpdateAirSpeed(vfr_hud.airspeed);
          emit UpdateGroundSpeed(vfr_hud.groundspeed);
          emit UpdateHUD(vfr_hud);
          ui->uav->setThrottle(vfr_hud.throttle);
          ui->uav_mag->append("mavlink_vfr_hud_t");
          break;
        // Power Status - no test
        /*
        case MAVLINK_MSG_ID_POWER_STATUS:
          mavlink_power_status_t power_status;
          mavlink_msg_power_status_decode(&msg, &power_status);
          emit UpdatePowerStatus(power_status);
          ui->uav_mag->append("mavlink_power_status_t");
          break;
        // Nav Ctl Output - no test
        case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:
          mavlink_nav_controller_output_t nav_ctl_out;
          mavlink_msg_nav_controller_output_decode(&msg, &nav_ctl_out);
          emit UpdateNavControllerOutput(nav_ctl_out);
          ui->uav_mag->append("mavlink_nav_controller_output_t");
          break;

        // Rc Channels - no test
        case MAVLINK_MSG_ID_RC_CHANNELS:
          mavlink_rc_channels_t rc_channels;
          mavlink_msg_rc_channels_decode(&msg, &rc_channels);
          //emit UpdateRcChannels(rc_channels);
          ui->uav_mag->append("mavlink_rc_channels_t");
          break;
        // Raw imu - no test
        case MAVLINK_MSG_ID_RAW_IMU:
          mavlink_raw_imu_t raw_imu;
          mavlink_msg_raw_imu_decode(&msg, &raw_imu);
          emit UpdateRawIMU(raw_imu);
          ui->uav_mag->append("mavlink_raw_imu_t");
          break;
        */
        case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:
          mavlink_servo_output_raw_t servo_out;
          mavlink_msg_servo_output_raw_decode(&msg, &servo_out);
          /**
           * roll: chan1_raw
           * pitch: chan2_raw
           * throttle: chan3_raw
           * yaw: chan4_raw
           */
          emit UpdateServoOutput(servo_out);
          ui->uav_mag->append("mavlink_servo_output_raw_t");
          break;
        /*
        case MAVLINK_MSG_ID_RADIO_STATUS:
          mavlink_radio_status_t radio_status;
          mavlink_msg_radio_status_decode(&msg, &radio_status);
          ui->uav_mag->append("mavlink_radio_status_t");
          break;
        */
        default:
          break;
      }
    }
  }
}

/**
 * @brief Read serial data
 *
 * @note This slot fun connect QSerialPort::readyRead signal
 */
void Connection::ReadData() {
  QByteArray in_data = serial_->readAll();
  HandleMessage(in_data);
}


/**
 * @brief Write byte array data to serial
 */
void Connection::WriteData(const QByteArray &write_data) {
  serial_->write(write_data);
}



/**
 * @brief Read udp data
 * @note receive ok
 */
void Connection::ReadyRead() {
  int read_length = socket_->pendingDatagramSize();
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  QHostAddress sender;
  uint16_t senderPort;
  // read msg
  // Note: (char*), no (unsign char*)
  socket_->readDatagram((char*)buf, read_length, &sender, &senderPort);

  QScrollBar *scroll_bar = nullptr;

  // decoding
  mavlink_message_t msg;
  mavlink_status_t status;
  static int heartbeat_time = 0;
#ifdef SERIAL_READ_ALL_ONCE
  for (int i = 0; i < read_length; i++) {
    // What is MAVLINK_COMM_0 ?
    if (mavlink_parse_char(MAVLINK_COMM_0, buf[i], &msg, &status)) {
      switch (msg.msgid) {
        case MAVLINK_MSG_ID_COMMAND_ACK:
          mavlink_command_ack_t cmd_ack;
          mavlink_msg_command_ack_decode(&msg, &cmd_ack);
          emit UpdateCmdAck(cmd_ack);
          ui->uav_mag->append("mavlink_command_ack_t");
          break;
        // Heartbeat
        case MAVLINK_MSG_ID_HEARTBEAT:
          mavlink_heartbeat_t heartbeat;
          // decode MAVLink msg
          // Bug: Why Mavlink version is 3 ?
          mavlink_msg_heartbeat_decode(&msg, &heartbeat);

          // Update connect uav ui
          ui->uav->UpdateUavInfo(heartbeat);

          //emit UpdateUavInfo(heartbeat);
          emit UpdateMavMode(heartbeat.base_mode);
          ui->uav_mag->append("mavlink_heartbeat_t");
          break;
        // Attitude
        case MAVLINK_MSG_ID_ATTITUDE:
          mavlink_attitude_t attitude;
          mavlink_msg_attitude_decode(&msg, &attitude);
          // emit signal to update UI
          emit UpdateAttitude(attitude);
          ui->uav_mag->append("mavlink_attitude_t");
          break;
        // Attitude
        case MAVLINK_MSG_ID_ALTITUDE:
          mavlink_altitude_t altitude;
          mavlink_msg_altitude_decode(&msg, &altitude);
          emit UpdateAltitude(altitude);
          ui->uav_mag->append("mavlink_altitude_t");
          break;
        // Battery
        case MAVLINK_MSG_ID_BATTERY_STATUS:
          mavlink_battery_status_t battery;
          mavlink_msg_battery_status_decode(&msg, &battery);

          ui->uav->setBatteryRemaining(battery.battery_remaining);
          ui->uav_mag->append("mavlink_battery_status_t");
          break;
        // Control system state
        case MAVLINK_MSG_ID_CONTROL_SYSTEM_STATE:
          mavlink_control_system_state_t ctl_sys_state;
          mavlink_msg_control_system_state_decode(&msg, &ctl_sys_state);
          emit UpdateAirSpeed(ctl_sys_state.airspeed);
          ui->uav_mag->append("mavlink_control_system_state_t");
          break;
        // GPS
        case MAVLINK_MSG_ID_GPS_RAW_INT:
          mavlink_gps_raw_int_t gps;
          mavlink_msg_gps_raw_int_decode(&msg, &gps);
          //emit UpdateGPS(gps);
          ui->uav_mag->append("mavlink_gps_raw_int_t");
          break;
        // Sys Status
        case MAVLINK_MSG_ID_SYS_STATUS:
          mavlink_sys_status_t sys_status;
          mavlink_msg_sys_status_decode(&msg, &sys_status);
          emit UpdateSysStatusSensor(sys_status);
          ui->uav_mag->append("mavlink_sys_status_t");
          break;
        // Gps Status
        case MAVLINK_MSG_ID_GPS_STATUS:
          mavlink_gps_status_t gps_status;
          mavlink_msg_gps_status_decode(&msg, &gps_status);
          // Note: Don't sure satellites_visible -> gps_use
          //emit UpdateGPSStatus(gps_status.satellites_visible);
          ui->uav_mag->append("mavlink_gps_status_t");
          break;
        // Global Position
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
          mavlink_global_position_int_t global_position;
          mavlink_msg_global_position_int_decode(&msg, &global_position);
          emit UpdateGlobalPositionInt(global_position);
          ui->uav_mag->append("mavlink_global_position_int_t");
          break;
        // HUD: air_speed and ground speed
        case MAVLINK_MSG_ID_VFR_HUD:
          mavlink_vfr_hud_t vfr_hud;
          mavlink_msg_vfr_hud_decode(&msg, &vfr_hud);
          emit UpdateAirSpeed(vfr_hud.airspeed);
          emit UpdateGroundSpeed(vfr_hud.groundspeed);
          emit UpdateHUD(vfr_hud);
          ui->uav_mag->append("mavlink_vfr_hud_t");
          break;
        default:
          break;
      }
    }
  }
#else
  uint8_t read_byte = 0;
  while (serial_->bytesAvailable() > 0) {
    if (serial_->read(&read_byte, 1) > 0) {
      // What is MAVLINK_COMM_0 ?
      if (mavlink_parse_char(MAVLINK_COMM_0, read_byte, &msg, &status)) {
        switch (msg.msgid) {
          case MAVLINK_MSG_ID_HEARTBEAT:
            mavlink_heartbeat_t heartbeat;
            mavlink_msg_heartbeat_decode(&msg, &heartbeat);
            // emit signal to update UI
            break;
          default:
            break;
        }
      }
    }
  }
#endif



}


/**
 * @brief Timer Update
 * @note Send unlock pack to APM every 100 ms
 */
void Connection::SendUnlockPack() {
  WriteData(unlock_pack_);
}


/**
 * @brief Timer Update
 * @note Send mavlink_heartbeat_t to APM every 1 seconds
 */
void Connection::TimerUpdate() {
  mavlink_message_t msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];
  //uint8_t system_id = 1;
  //uint8_t component_id = 1;

  // init heartbeat
  mavlink_heartbeat_t heartbeat;
  heartbeat.mavlink_version = 1;
  heartbeat.autopilot = MAV_AUTOPILOT_ARDUPILOTMEGA;
  heartbeat.base_mode = MAV_MODE_FLAG_STABILIZE_ENABLED;
  heartbeat.custom_mode = MAV_MODE_FLAG_STABILIZE_ENABLED;
  heartbeat.system_status = MAV_STATE_BOOT;
  heartbeat.type = MAV_TYPE_GCS;


  // encode MAVLink msg, this msg need to be send to UAV every 1 s
  mavlink_msg_heartbeat_encode(kSystemId, kComponentId, &msg, &heartbeat);
  // send to buf
  uint8_t len = mavlink_msg_to_send_buffer(buf, &msg);

  // copy to byte array from buf
  QByteArray out_data;
  out_data.clear();
  for (int i = 0; i < len; i++) {
    out_data.append(buf[i]);
  }

  // I don`t know this function whether to be used
  // QByteArray::fromRawData(buf, len);

  WriteData(out_data);
  //emit WriteMessage(out_data);
}

/**
 * @brief send mavlink cmd to APM
 * @param cmd type: un_lock, set_height, etc...
 */
void Connection::SendMAVLinkCmd(const mavlink_command_long_t *cmd_packet) {
  mavlink_message_t cmd_msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  // Note: cmd_packet param is a pointer, no reference!
  mavlink_msg_command_long_encode(kSystemId, kComponentId, &cmd_msg, cmd_packet);

  // send to buf
  uint8_t len = mavlink_msg_to_send_buffer(buf, &cmd_msg);

  // copy to byte array from buf
  QByteArray out_data;
  out_data.clear();
  for (int i = 0; i < len; i++) {
    out_data.append(buf[i]);
  }

  // write cmd msg to serial
  WriteData(out_data);
  //emit WriteMessage(out_data);
}



void Connection::SendRcChannels(const mavlink_rc_channels_override_t *rc_override) {
  mavlink_message_t rc_channels_msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  // Note: cmd_packet param is a pointer, no reference!
  mavlink_msg_rc_channels_override_encode(kSystemId, kComponentId, &rc_channels_msg, rc_override);
  //mavlink_msg_rc_channels_override_pack(255, 1)
  // send to buf
  uint8_t len = mavlink_msg_to_send_buffer(buf, &rc_channels_msg);

  // copy to byte array from buf
  QByteArray out_data;
  out_data.clear();
  for (int i = 0; i < len; i++) {
    out_data.append(buf[i]);
  }

  // write cmd msg to serial
  WriteData(out_data);
  //emit WriteMessage(out_data);
}


void Connection::SendUnlockSignal(const mavlink_rc_channels_override_t *rc_unlock) {
  mavlink_message_t rc_channels_msg;
  uint8_t buf[MAVLINK_MAX_PACKET_LEN];

  // Note: cmd_packet param is a pointer, no reference!
  mavlink_msg_rc_channels_override_encode(kSystemId, kComponentId, &rc_channels_msg, rc_unlock);
  //mavlink_msg_rc_channels_override_pack(255, 1)
  // send to buf
  uint8_t len = mavlink_msg_to_send_buffer(buf, &rc_channels_msg);

  // copy to byte array from buf
  unlock_pack_.clear();
  for (int i = 0; i < len; i++) {
    unlock_pack_.append(buf[i]);
  }

  // write cmd msg to serial
  //WriteData(out_data);
  unlock_timer_->start(10);
}


void Connection::StopSendUnlockPack(const uint8_t base_mode) {
  if (base_mode & MAV_MODE_FLAG_SAFETY_ARMED) {
    if (unlock_timer_->isActive()) {
      unlock_timer_->stop();
    }
  }
}


void Connection::UpdateUavInfoSlot(const mavlink_heartbeat_t &heartbeat) {
  ui->uav->UpdateUavInfo(heartbeat);
  ui->uav_mag->append("mavlink_heartbeat_t");

  emit UpdateARM(heartbeat.base_mode);
  emit UpdateMavMode(heartbeat.custom_mode);
}

void Connection::UpdateAttitudeSlot(const mavlink_attitude_t &attitude) {
  ui->uav_mag->append("mavlink_attitude_t");

  emit UpdateAttitude(attitude);
}

void Connection::UpdateBatteryStatusSlot(const mavlink_battery_status_t &battery) {
  ui->uav->setBatteryRemaining(battery.battery_remaining);
  ui->uav_mag->append("mavlink_battery_status_t");

  emit UpdateBatteryRemain(battery.battery_remaining);
}

void Connection::UpdateSysStatusSensorSlot(const mavlink_sys_status_t &sys_status) {
  ui->uav_mag->append("mavlink_sys_status_t");
  emit UpdateSysStatusSensor(sys_status);
}

void Connection::UpdateGlobalPositionIntSlot(const mavlink_global_position_int_t &global_position) {
  ui->uav_mag->append("mavlink_global_position_int_t");

  // Gps status
  if ((global_position.lat == 0) && (global_position.lon == 0)) {
    emit UpdateGPSStatus(false);
  } else {
    emit UpdateGPSStatus(true);
  }

  emit UpdateAlt(global_position.alt);
  emit UpdateGlobalPositionInt(global_position);
}

void Connection::UpdateHUDSlot(const mavlink_vfr_hud_t &vfr_hud) {
  ui->uav->setThrottle(vfr_hud.throttle);
  ui->uav_mag->append("mavlink_vfr_hud_t");
  emit UpdateAirSpeed(vfr_hud.airspeed);
  emit UpdateGroundSpeed(vfr_hud.groundspeed);
  emit UpdateHUD(vfr_hud);
}

void Connection::UpdateServoOutputSlot(const mavlink_servo_output_raw_t &servo_out) {
  ui->uav_mag->append("mavlink_servo_output_raw_t");
  emit UpdateServoOutput(servo_out);
}


