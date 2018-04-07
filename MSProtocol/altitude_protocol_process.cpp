#include "altitude_protocol_process.h"

AltitudeProtocolProcess::AltitudeProtocolProcess()
{

}


AltitudeProtocolProcess::~AltitudeProtocolProcess()
{

}


/**
* Function name: MspAltitudeDownToDC
* Function: turn msp protocol data to real data so that we can show they in UI.
* Params: void
* Return: void
* Note: not divided by the conversion factor.
* Time: 2018/4/7 cdeveloper
*/
void AltitudeProtocolProcess::MspAltitudeDownToDC() {
  altitude_down_dc_.message_header = altitude_down_.message_header;
  altitude_down_dc_.est_alt = static_cast<double>(altitude_down_.est_alt);
  altitude_down_dc_.vario = static_cast<double>(altitude_down_.vario);
  altitude_down_dc_.crc = altitude_down_.crc;
}

/**
* Function name: Pack
* Function: upstream pack method for MSP_ALTITUDE
* Params: the refer of the pack to be send
* Return: pack success return true else false
* Note: there is no check packet now !
* Time: 2018/4/7 cdeveloper
*/
bool AltitudeProtocolProcess::Pack(QByteArray &out_data) {
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
  out_data.append(static_cast<char>(MSP_ALTITUDE));
  // crc
  out_data.append(static_cast<char>(0 ^ MSP_ALTITUDE));
  return true;
}

/**
* Function name: UnPack
* Function: downstream unpack method for MSP_ALTITUDE.
* Params: the refer of the in data packget.
* Return: unpack success return true else false.
* Note: there is no check packet now !
* Time: 2018/4/6 cdeveloper
*/
bool AltitudeProtocolProcess::UnPack(const QByteArray &in_data) {
  GetMessageHeaderInfo(in_data, altitude_down_.message_header);

  // msg header is 5 bytes.
  int data_start_index = sizeof(struct MessageHeader);

  altitude_down_.est_alt = (in_data.at(data_start_index) |
                            (in_data.at(data_start_index + 1) << 8) |
                            (in_data.at(data_start_index + 2) << 16)|
                            (in_data.at(data_start_index + 3) << 24));

  altitude_down_.vario = (in_data.at(data_start_index + 4) | (in_data.at(data_start_index + 5) << 8));
  altitude_down_.crc = in_data.at(data_start_index + 6);
  return true;
}











