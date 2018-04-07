#ifndef MOTORPINS_PROTOCOL_PROCESS_H
#define MOTORPINS_PROTOCOL_PROCESS_H

#include "msp_protocol_process_interface.h"
#include "MSProtocol/msp_protocol_convert_to_real_data.h"
#include "MSProtocol/msp_protocol_structs.h"


class MotorPinsProtocolProcess : public MSProtocolProcessInterface
{
public:
  MotorPinsProtocolProcess();
  ~MotorPinsProtocolProcess();

  MspMotorPinsDown motor_pin_down_;
  MspMotorDownDC motor_pin_down_dc_;

  void MspMotorDownToDC();

  virtual bool Pack(QByteArray &out_data) override;
  virtual bool UnPack(const QByteArray &in_data) override;
};

#endif // MOTORPINS_PROTOCOL_PROCESS_H
