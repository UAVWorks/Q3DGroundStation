#include "rctuning_protocol_process.h"

RcTuningProtocolProcess::RcTuningProtocolProcess()
{

}


RcTuningProtocolProcess::~RcTuningProtocolProcess()
{

}

/**
* Function name: RcTuningProtocolProcess
* Function: turn msp protocol data to real data so that we can show they in UI.
* Params: void
* Return: void
* Note: not divided by the conversion factor.
* Time: 2018/4/7 cdeveloper
*/
void RcTuningProtocolProcess::MspRcTuningDownToDC() {
  rctuning_down_dc_.message_header = rctuning_down_.message_header;
  rctuning_down_dc_.byte_rc_rate = static_cast<double>(rctuning_down_.byte_rc_rate);
  rctuning_down_dc_.byte_rc_expo = static_cast<double>(rctuning_down_.byte_rc_expo);
  rctuning_down_dc_.byte_roll_pitch_rate = static_cast<double>(rctuning_down_.byte_roll_pitch_rate);
  rctuning_down_dc_.byte_yaw_rate = static_cast<double>(rctuning_down_.byte_yaw_rate);
  rctuning_down_dc_.byte_dynthr_pid = static_cast<double>(rctuning_down_.byte_dynthr_pid);
  rctuning_down_dc_.byte_throttle_mid = static_cast<double>(rctuning_down_.byte_throttle_mid);
  rctuning_down_dc_.byte_throttle_expo = static_cast<double>(rctuning_down_.byte_throttle_expo);
  rctuning_down_dc_.crc = rctuning_down_.crc;
}

/**
* Function name: Pack
* Function: upstream pack method for MSP_RC_TUNING
* Params: the refer of the pack to be send
* Return: pack success return true else false
* Note: there is no check packet now !
* Time: 2018/4/7 cdeveloper
*/
bool RcTuningProtocolProcess::Pack(QByteArray &out_data) {
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
  out_data.append(static_cast<char>(MSP_RC_TUNING));
  // crc
  out_data.append(static_cast<char>(0 ^ MSP_RC_TUNING));
  return true;
}

/**
* Function name: UnPack
* Function: downstream unpack method for MSP_RC_TUNING.
* Params: the refer of the in data packget.
* Return: unpack success return true else false.
* Note: there is no check packet now !
* Time: 2018/4/7 cdeveloper
*/
bool RcTuningProtocolProcess::UnPack(const QByteArray &in_data) {
  GetMessageHeaderInfo(in_data, rctuning_down_.message_header);

  // msg header is 5 bytes.
  int data_start_index = sizeof(struct MessageHeader);

  rctuning_down_.byte_rc_rate = in_data.at(data_start_index);
  rctuning_down_.byte_rc_expo = in_data.at(data_start_index + 1);
  rctuning_down_.byte_roll_pitch_rate = in_data.at(data_start_index + 2);
  rctuning_down_.byte_yaw_rate = in_data.at(data_start_index + 3);
  rctuning_down_.byte_dynthr_pid = in_data.at(data_start_index + 4);
  rctuning_down_.byte_throttle_mid = in_data.at(data_start_index + 5);
  rctuning_down_.byte_throttle_expo = in_data.at(data_start_index + 6);

  rctuning_down_.crc = in_data.at(data_start_index + 7);
  return true;
}
