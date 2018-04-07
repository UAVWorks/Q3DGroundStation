#ifndef PID_PROTOCOL_PROCESS_H
#define PID_PROTOCOL_PROCESS_H

#include "msp_protocol_process_interface.h"
#include "MSProtocol/msp_protocol_convert_to_real_data.h"
#include "MSProtocol/msp_protocol_structs.h"

class PIDProtocolProcess : public MSProtocolProcessInterface
{
public:
  PIDProtocolProcess();
  ~PIDProtocolProcess();

  MspPIDDown pid_down_;
  MspPIDDownDC pid_down_dc_;

  void MspPIDDownToDC();

  virtual bool Pack(QByteArray &out_data) override;
  virtual bool UnPack(const QByteArray &in_data) override;
};

#endif // PID_PROTOCOL_PROCESS_H
