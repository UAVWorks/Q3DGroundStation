#include "analog_protocol_process.h"

AnalogProtocolProcess::AnalogProtocolProcess() {

}

AnalogProtocolProcess::~AnalogProtocolProcess() {

}


/**
* Function name: AnalogProtocolProcess
* Function: turn msp protocol data to real data so that we can show they in UI.
* Params: void
* Return: void
* Note: not divided by the conversion factor.
* Time: 2018/4/6 cdeveloper
*/
void AnalogProtocolProcess::MspAnalogDownToDC() {
  analog_down_dc_.message_header = analog_down_.message_header;
  analog_down_dc_.vbat = static_cast<double>(analog_down_.vbat);
  analog_down_dc_.int_power_meter_sum = static_cast<double>(analog_down_.int_power_meter_sum);
  analog_down_dc_.rssi = static_cast<double>(analog_down_.rssi);
  analog_down_dc_.amperage = static_cast<double>(analog_down_.amperage);
  analog_down_dc_.crc = analog_down_.crc;
}


/**
* Function name: Pack
* Function: upstream pack method for MSP_ANALOG
* Params: the refer of the pack to be send
* Return: pack success return true else false
* Note: there is no check packet now !
* Time: 2018/4/6 cdeveloper
*/
bool AnalogProtocolProcess::Pack(QByteArray &out_data) {
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
  out_data.append(static_cast<char>(MSP_ANALOG));
  // crc
  out_data.append(static_cast<char>(0 ^ MSP_ANALOG));

  return true;
}

bool AnalogProtocolProcess::UnPack(const QByteArray &in_data) {
  GetMessageHeaderInfo(in_data, analog_down_.message_header);

  // msg header is 5 bytes.
  int data_start_index = sizeof(struct MessageHeader);

  analog_down_.vbat = (in_data.at(data_start_index));
  analog_down_.int_power_meter_sum = (in_data.at(data_start_index + 1) | in_data.at(data_start_index + 2) << 8);
  analog_down_.rssi = (in_data.at(data_start_index + 3) | in_data.at(data_start_index + 4) << 8);
  analog_down_.amperage = (in_data.at(data_start_index + 5) | in_data.at(data_start_index + 6) << 8);
  analog_down_.crc = in_data.at(data_start_index + 7);
  return true;
}
