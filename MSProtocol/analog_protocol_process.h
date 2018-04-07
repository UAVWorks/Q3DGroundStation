#ifndef ANALOG_PROTOCOL_PROCESS_H
#define ANALOG_PROTOCOL_PROCESS_H

#include "msp_protocol_process_interface.h"
#include "MSProtocol/msp_protocol_convert_to_real_data.h"
#include "MSProtocol/msp_protocol_structs.h"


class AnalogProtocolProcess : public MSProtocolProcessInterface
{
public:
  AnalogProtocolProcess();
  ~AnalogProtocolProcess();

  MspAnalogDown analog_down_;
  MspAltitudeDownDC analog_down_dc_;

  void MspAnalogDownToDC();

  virtual bool Pack(QByteArray &out_data) override;
  virtual bool UnPack(const QByteArray &in_data);
};

#endif // ANALOG_PROTOCOL_PROCESS_H
