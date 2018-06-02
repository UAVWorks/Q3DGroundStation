#include "serial_read_thread.h"

ReadSerialController::ReadSerialController(QSerialPort *serial) {
  ReadSerialWorker *read_worker = new ReadSerialWorker(serial);
  read_worker->moveToThread(&read_thread);

  connect(serial, &QSerialPort::readyRead, read_worker, &ReadSerialWorker::StartWork);
  connect(read_worker, &ReadSerialWorker::MessageReady, this, &ReadSerialController::HandleMessage);
  connect(&read_thread, &QThread::finished, read_worker, &QObject::deleteLater);

  read_thread.start();
}

ReadSerialController::~ReadSerialController() {
  read_thread.quit();
  read_thread.wait();
}
#if PARSE_MSG_IN_THREAD
void ReadSerialController::HandleMessage(const mavlink_message_t &read_msg) {
  switch (read_msg.msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT:
      mavlink_heartbeat_t heartbeat;
      mavlink_msg_heartbeat_decode(&read_msg, &heartbeat);
      emit UpdateUavInfo(heartbeat);
      break;
    case MAVLINK_MSG_ID_ATTITUDE:
      mavlink_attitude_t attitude;
      mavlink_msg_attitude_decode(&read_msg, &attitude);
      emit UpdateAttitude(attitude);
      break;
    case MAVLINK_MSG_ID_SYS_STATUS:
      mavlink_sys_status_t sys_status;
      mavlink_msg_sys_status_decode(&read_msg, &sys_status);
      emit UpdateSysStatusSensor(sys_status);
      break;
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
      mavlink_global_position_int_t global_position;
      mavlink_msg_global_position_int_decode(&read_msg, &global_position);
      emit UpdateGlobalPositionInt(global_position);
      break;
    case MAVLINK_MSG_ID_VFR_HUD:
      mavlink_vfr_hud_t vfr_hud;
      mavlink_msg_vfr_hud_decode(&read_msg, &vfr_hud);
      emit UpdateHUD(vfr_hud);
      break;
    case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:
      mavlink_servo_output_raw_t servo_out;
      mavlink_msg_servo_output_raw_decode(&read_msg, &servo_out);
      emit UpdateServoOutput(servo_out);
      break;
    default:
      break;
  }
}
#else
void ReadSerialController::HandleMessage(const QByteArray &raw_msg) {
  mavlink_message_t msg;
  mavlink_status_t status;

  for (int i = 0; i < raw_msg.length(); i++) {
    if (mavlink_parse_char(MAVLINK_COMM_0, raw_msg[i], &msg, &status)) {
      switch (msg.msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT:
          mavlink_heartbeat_t heartbeat;
          mavlink_msg_heartbeat_decode(&msg, &heartbeat);
          emit UpdateUavInfo(heartbeat);
          break;
        case MAVLINK_MSG_ID_ATTITUDE:
          mavlink_attitude_t attitude;
          mavlink_msg_attitude_decode(&msg, &attitude);
          emit UpdateAttitude(attitude);
          break;
        case MAVLINK_MSG_ID_SYS_STATUS:
          mavlink_sys_status_t sys_status;
          mavlink_msg_sys_status_decode(&msg, &sys_status);
          emit UpdateSysStatusSensor(sys_status);
          break;
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
          mavlink_global_position_int_t global_position;
          mavlink_msg_global_position_int_decode(&msg, &global_position);
          emit UpdateGlobalPositionInt(global_position);
          break;
        case MAVLINK_MSG_ID_VFR_HUD:
          mavlink_vfr_hud_t vfr_hud;
          mavlink_msg_vfr_hud_decode(&msg, &vfr_hud);
          emit UpdateHUD(vfr_hud);
          break;
        case MAVLINK_MSG_ID_SERVO_OUTPUT_RAW:
          mavlink_servo_output_raw_t servo_out;
          mavlink_msg_servo_output_raw_decode(&msg, &servo_out);
          emit UpdateServoOutput(servo_out);
          break;
        default:
          break;
      }
    }
  }
}
#endif
