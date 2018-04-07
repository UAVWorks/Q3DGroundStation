#include "motor_protocol_process.h"

MotorProtocolProcess::MotorProtocolProcess()
{

}


MotorProtocolProcess::~MotorProtocolProcess()
{

}

bool MotorProtocolProcess::Pack(QByteArray &out_data) {
  out_data.clear();

  // set out_data MSP_MOTOR = 104
  out_data.append(kHeaderStart);
  out_data.append(kHeaderM);
  out_data.append(kGcsToUav);
  // size = 0
  out_data.append(static_cast<char>(0));
  // cmd = 104
  out_data.append(static_cast<char>(MSP_MOTOR));
  // crc = 0 ^ 104
  out_data.append(static_cast<char>(0 ^ MSP_MOTOR));

  return true;
}

/*
bool MotorProtocolProcess::UnPack(BaseData *base_data, const std::vector<uint8_t> in_packget) {

  return true;
}
*/



bool MotorProtocolProcess::UnPack(const QByteArray &in_data) {
  // unpack header
  GetMessageHeaderInfo(in_data, motor_down_.message_header);

  // msg header is 5 byte
  int headr_len = sizeof(struct MessageHeader);
  int data_start_index = headr_len;

  // unpack data. NOTE: LSB
  // NOTE: The motor position is uncertain !!!
  motor_down_.lt_motor = (in_data.at(data_start_index) | (in_data.at(data_start_index + 1) << 8));
  motor_down_.rt_motor = (in_data.at(data_start_index + 2) | (in_data.at(data_start_index + 3) << 8));
  motor_down_.lb_motor = (in_data.at(data_start_index + 4) | (in_data.at(data_start_index + 5) << 8));
  motor_down_.rb_motor = (in_data.at(data_start_index + 6) | (in_data.at(data_start_index + 7) << 8));

  motor_down_.unuse_one = 0;
  motor_down_.unuse_two = 0;
  motor_down_.unuse_three = 0;
  motor_down_.unuse_four = 0;

  motor_down_.crc = in_data.at(data_start_index + 16);
  return true;
}

void MotorProtocolProcess::MspMotorDownToDC() {
  motor_down_dc.message_header = motor_down_.message_header;
  // NOTE: May be add convert factor!!!
  // May be no 100.0
  motor_down_dc.lt_motor = static_cast<double>(motor_down_.lt_motor) / 1000.0;
  motor_down_dc.rt_motor = static_cast<double>(motor_down_.rt_motor) / 1000.0;
  motor_down_dc.lb_motor = static_cast<double>(motor_down_.lb_motor) / 1000.0;
  motor_down_dc.rb_motor = static_cast<double>(motor_down_.rb_motor) / 1000.0;
  motor_down_dc.unuse_one = 0.0;
  motor_down_dc.unuse_two = 0.0;
  motor_down_dc.unuse_three = 0.0;
  motor_down_dc.unuse_four = 0.0;
}


