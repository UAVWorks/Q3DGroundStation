#include "rc_protocol_process.h"

RcProtocolProcess::RcProtocolProcess() {

}


RcProtocolProcess::~RcProtocolProcess() {

}


/**
* Function name: MspRcDownToDC
* Function: turn msp protocol data to real data so that we can show they in UI.
* Params: void
* Return: void
* Note: not divided by the conversion factor.
* Time: 2018/4/6 cdeveloper
*/
void RcProtocolProcess::MspRcDownToDC() {
  rc_down_dc_.message_header = rc_down_.message_header;
  rc_down_dc_.roll = static_cast<double>(rc_down_.roll);
  rc_down_dc_.pitch = static_cast<double>(rc_down_.pitch);
  rc_down_dc_.yaw = static_cast<double>(rc_down_.yaw);
  rc_down_dc_.throttle = static_cast<double>(rc_down_.throttle);
  rc_down_dc_.aux1 = static_cast<double>(rc_down_.aux1);
  rc_down_dc_.aux2 = static_cast<double>(rc_down_.aux2);
  rc_down_dc_.aux3 = static_cast<double>(rc_down_.aux3);
  rc_down_dc_.aux4 = static_cast<double>(rc_down_.aux4);
  rc_down_dc_.crc = rc_down_.crc;
}


/**
* Function name: Pack
* Function: upstream pack method for MSP_RC
* Params: the refer of the pack to be send
* Return: pack success return true else false
* Note: there is no check packet now !
* Time: 2018/4/6 cdeveloper
*/
bool RcProtocolProcess::Pack(QByteArray &out_data) {
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
  out_data.append(static_cast<char>(MSP_RC));
  // crc
  out_data.append(static_cast<char>(0 ^ MSP_RC));
  return true;
}


/**
* Function name: UnPack
* Function: downstream unpack method for MSP_RC.
* Params: the refer of the in data packget.
* Return: unpack success return true else false.
* Note: there is no check packet now !
* Time: 2018/4/6 cdeveloper
*/
bool RcProtocolProcess::UnPack(const QByteArray &in_data) {
  GetMessageHeaderInfo(in_data, rc_down_.message_header);

  // msg header is 5 bytes.
  int data_start_index = sizeof(struct MessageHeader);

  rc_down_.roll = (in_data.at(data_start_index) | (in_data.at(data_start_index + 1) << 8));
  rc_down_.pitch = (in_data.at(data_start_index + 2) | (in_data.at(data_start_index + 3) << 8));
  rc_down_.yaw = (in_data.at(data_start_index + 4) | (in_data.at(data_start_index + 5) << 8));
  rc_down_.throttle = (in_data.at(data_start_index + 6) | (in_data.at(data_start_index + 7) << 8));
  rc_down_.aux1 = (in_data.at(data_start_index + 8) | (in_data.at(data_start_index + 9) << 8));
  rc_down_.aux2 = (in_data.at(data_start_index + 10) | (in_data.at(data_start_index + 11) << 8));
  rc_down_.aux3 = (in_data.at(data_start_index + 12) | (in_data.at(data_start_index + 13) << 8));
  rc_down_.aux4 = (in_data.at(data_start_index + 14) | (in_data.at(data_start_index + 15) << 8));

  rc_down_.crc = in_data.at(data_start_index + 16);
  return true;
}










