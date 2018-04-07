#ifndef ALTITUDE_PROTOCOL_PROCESS_H
#define ALTITUDE_PROTOCOL_PROCESS_H

#include "msp_protocol_process_interface.h"
#include "MSProtocol/msp_protocol_convert_to_real_data.h"
#include "MSProtocol/msp_protocol_structs.h"


class AltitudeProtocolProcess : public MSProtocolProcessInterface
{
public:
  AltitudeProtocolProcess();
  ~AltitudeProtocolProcess();

  MspAltitudeDown altitude_down_;
  MspAltitudeDownDC altitude_down_dc_;

  void MspAltitudeDownToDC();

  virtual bool Pack(QByteArray &out_data) override;
  virtual bool UnPack(const QByteArray &in_data);
};

#endif // ALTITUDE_PROTOCOL_PROCESS_H
