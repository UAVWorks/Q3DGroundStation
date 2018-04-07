#ifndef MOTOR_PROTOCOL_PROCESS_H
#define MOTOR_PROTOCOL_PROCESS_H

#include "msp_protocol_process_interface.h"
#include "MSProtocol/msp_protocol_convert_to_real_data.h"
#include "MSProtocol/msp_protocol_structs.h"

class MotorProtocolProcess : public MSProtocolProcessInterface
{
public:
  MotorProtocolProcess();
  ~MotorProtocolProcess();

  MspMotorDown motor_down_;
  MspMotorDownDC motor_down_dc;

  void MspMotorDownToDC();

  virtual bool Pack(QByteArray &out_data) override;
  //virtual bool UnPack(BaseData *base_data, const std::vector<uint8_t> in_packget) override;
  virtual bool UnPack(const QByteArray &in_data) override;
};

#endif // MOTOR_PROTOCOL_PROCESS_H
