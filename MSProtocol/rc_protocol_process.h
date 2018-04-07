#ifndef RC_PROTOCOL_PROCESS_H
#define RC_PROTOCOL_PROCESS_H

#include "msp_protocol_process_interface.h"
#include "MSProtocol/msp_protocol_convert_to_real_data.h"
#include "MSProtocol/msp_protocol_structs.h"

class RcProtocolProcess : public MSProtocolProcessInterface
{
public:
  RcProtocolProcess();
  ~RcProtocolProcess();

  MspRcDown rc_down_;
  MspRcDownDC rc_down_dc_;

  void MspRcDownToDC();

  virtual bool Pack(QByteArray &out_data) override;
  virtual bool UnPack(const QByteArray &in_data) override;
};

#endif // RC_PROTOCOL_PROCESS_H
