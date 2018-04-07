#ifndef RCTUNING_PROTOCOL_PROCESS_H
#define RCTUNING_PROTOCOL_PROCESS_H

#include "msp_protocol_process_interface.h"
#include "MSProtocol/msp_protocol_convert_to_real_data.h"
#include "MSProtocol/msp_protocol_structs.h"

class RcTuningProtocolProcess : public MSProtocolProcessInterface
{
public:
  RcTuningProtocolProcess();
  ~RcTuningProtocolProcess();

  MspRcTuningDown rctuning_down_;
  MspRcTuningDownDC rctuning_down_dc_;

  void MspRcTuningDownToDC();

  virtual bool Pack(QByteArray &out_data) override;
  virtual bool UnPack(const QByteArray &in_data) override;
};

#endif // RCTUNING_PROTOCOL_PROCESS_H
