#ifndef RCDEADBANDS_PROTOCOL_PROCESS_H
#define RCDEADBANDS_PROTOCOL_PROCESS_H

#include "msp_protocol_process_interface.h"
#include "MSProtocol/msp_protocol_convert_to_real_data.h"
#include "MSProtocol/msp_protocol_structs.h"

class RcDeadbandsProtocolProcess : public MSProtocolProcessInterface
{
public:
  RcDeadbandsProtocolProcess();
  ~RcDeadbandsProtocolProcess();

  MspRcDeadbandsDown rcdeadbands_down_;
  MspRcDeadbandsDownDC rcdeadbands_down_dc_;

  void MspRcDeadbandsDownToDC();

  virtual bool Pack(QByteArray &out_data) override;
  virtual bool UnPack(const QByteArray &in_data) override;
};

#endif // RCDEADBANDS_PROTOCOL_PROCESS_H
