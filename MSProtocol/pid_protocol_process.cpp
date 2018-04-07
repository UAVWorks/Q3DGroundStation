#include "pid_protocol_process.h"

PIDProtocolProcess::PIDProtocolProcess() {

}


PIDProtocolProcess::~PIDProtocolProcess() {

}

/**
* Function name: MspPIDDownToDC
* Function: turn msp protocol data to real data so that we can show they in UI.
* Params: void
* Return: void
* Note: not divided by the conversion factor.
* Time: 2018/4/7 cdeveloper
*/
void PIDProtocolProcess::MspPIDDownToDC() {
  pid_down_dc_.message_header = pid_down_.message_header;
  // roll pid
  pid_down_dc_.roll[0] = static_cast<double>(pid_down_.roll[0]);
  pid_down_dc_.roll[1] = static_cast<double>(pid_down_.roll[1]);
  pid_down_dc_.roll[2] = static_cast<double>(pid_down_.roll[2]);

  // pitch pid
  pid_down_dc_.pitch[0] = static_cast<double>(pid_down_.pitch[0]);
  pid_down_dc_.pitch[1] = static_cast<double>(pid_down_.pitch[1]);
  pid_down_dc_.pitch[2] = static_cast<double>(pid_down_.pitch[2]);

  // yaw pid
  pid_down_dc_.yaw[0] = static_cast<double>(pid_down_.yaw[0]);
  pid_down_dc_.yaw[1] = static_cast<double>(pid_down_.yaw[1]);
  pid_down_dc_.yaw[2] = static_cast<double>(pid_down_.yaw[2]);

  // alt pid
  pid_down_dc_.alt[0] = static_cast<double>(pid_down_.alt[0]);
  pid_down_dc_.alt[1] = static_cast<double>(pid_down_.alt[1]);
  pid_down_dc_.alt[2] = static_cast<double>(pid_down_.alt[2]);

  // pos pid
  pid_down_dc_.pos[0] = static_cast<double>(pid_down_.pos[0]);
  pid_down_dc_.pos[1] = static_cast<double>(pid_down_.pos[1]);
  pid_down_dc_.pos[2] = static_cast<double>(pid_down_.pos[2]);

  // posr pid
  pid_down_dc_.posr[0] = static_cast<double>(pid_down_.posr[0]);
  pid_down_dc_.posr[1] = static_cast<double>(pid_down_.posr[1]);
  pid_down_dc_.posr[2] = static_cast<double>(pid_down_.posr[2]);

  // navr pid
  pid_down_dc_.navr[0] = static_cast<double>(pid_down_.navr[0]);
  pid_down_dc_.navr[1] = static_cast<double>(pid_down_.navr[1]);
  pid_down_dc_.navr[2] = static_cast<double>(pid_down_.navr[2]);

  // level pid
  pid_down_dc_.level[0] = static_cast<double>(pid_down_.level[0]);
  pid_down_dc_.level[1] = static_cast<double>(pid_down_.level[1]);
  pid_down_dc_.level[2] = static_cast<double>(pid_down_.level[2]);

  // mag pid
  pid_down_dc_.mag[0] = static_cast<double>(pid_down_.mag[0]);
  pid_down_dc_.mag[1] = static_cast<double>(pid_down_.mag[1]);
  pid_down_dc_.mag[2] = static_cast<double>(pid_down_.mag[2]);

  // vel pid
  pid_down_dc_.vel[0] = static_cast<double>(pid_down_.vel[0]);
  pid_down_dc_.vel[1] = static_cast<double>(pid_down_.vel[1]);
  pid_down_dc_.vel[2] = static_cast<double>(pid_down_.vel[2]);

  // crc
  pid_down_dc_.crc = pid_down_.crc;
}


/**
* Function name: Pack
* Function: upstream pack method for MSP_PID
* Params: the refer of the pack to be send
* Return: pack success return true else false
* Note: there is no check packet now !
* Time: 2018/4/7 cdeveloper
*/
bool PIDProtocolProcess::Pack(QByteArray &out_data) {
  out_data.clear();
  // $
  out_data.append(kHeaderStart);
  // M
  out_data.append(kHeaderM);
  // <
  out_data.append(kGcsToUav);
  // size
  out_data.append(static_cast<char>(0));
  // cmd
  out_data.append(static_cast<char>(MSP_PID));
  // crc
  out_data.append(static_cast<char>(0 ^ MSP_PID));

  return true;
}

/**
* Function name: UnPack
* Function: downstream unpack method for MSP_PID.
* Params: the refer of the in data packget.
* Return: unpack success return true else false.
* Note: there is no check packet now !
* Time: 2018/4/7 cdeveloper
*/
bool PIDProtocolProcess::UnPack(const QByteArray &in_data) {
  GetMessageHeaderInfo(in_data, pid_down_.message_header);

  // msg header is 5 bytes
  int data_start_index = sizeof(struct MessageHeader);

  // roll PID
  pid_down_.roll[0] = in_data.at(data_start_index++);
  pid_down_.roll[1] = in_data.at(data_start_index++);
  pid_down_.roll[2] = in_data.at(data_start_index++);

  // pitch PID
  pid_down_.pitch[0] = in_data.at(data_start_index++);
  pid_down_.pitch[1] = in_data.at(data_start_index++);
  pid_down_.pitch[2] = in_data.at(data_start_index++);

  // yaw PID
  pid_down_.yaw[0] = in_data.at(data_start_index++);
  pid_down_.yaw[1] = in_data.at(data_start_index++);
  pid_down_.yaw[2] = in_data.at(data_start_index++);

  // alt PID
  pid_down_.alt[0] = in_data.at(data_start_index++);
  pid_down_.alt[1] = in_data.at(data_start_index++);
  pid_down_.alt[2] = in_data.at(data_start_index++);

  // pos PID
  pid_down_.pos[0] = in_data.at(data_start_index++);
  pid_down_.pos[1] = in_data.at(data_start_index++);
  pid_down_.pos[2] = in_data.at(data_start_index++);

  // posr PID
  pid_down_.posr[0] = in_data.at(data_start_index++);
  pid_down_.posr[1] = in_data.at(data_start_index++);
  pid_down_.posr[2] = in_data.at(data_start_index++);

  // navr PID
  pid_down_.navr[0] = in_data.at(data_start_index++);
  pid_down_.navr[1] = in_data.at(data_start_index++);
  pid_down_.navr[2] = in_data.at(data_start_index++);

  // level PID
  pid_down_.level[0] = in_data.at(data_start_index++);
  pid_down_.level[1] = in_data.at(data_start_index++);
  pid_down_.level[2] = in_data.at(data_start_index++);

  // mag PID
  pid_down_.mag[0] = in_data.at(data_start_index++);
  pid_down_.mag[1] = in_data.at(data_start_index++);
  pid_down_.mag[2] = in_data.at(data_start_index++);

  // vel PID
  pid_down_.vel[0] = in_data.at(data_start_index++);
  pid_down_.vel[1] = in_data.at(data_start_index++);
  pid_down_.vel[2] = in_data.at(data_start_index++);

  pid_down_.crc = in_data.at(data_start_index++);
  return true;
}







