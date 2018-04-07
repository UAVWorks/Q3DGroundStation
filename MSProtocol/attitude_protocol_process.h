#ifndef ATTITUDE_PROTOCOL_PROCESS_H
#define ATTITUDE_PROTOCOL_PROCESS_H


#include <vector>

#include "msp_protocol_process_interface.h"
#include "MSProtocol/msp_protocol_convert_to_real_data.h"
#include "MSProtocol/msp_protocol_structs.h"

class AttitudeProtocolProcess : public MSProtocolProcessInterface
{
public:
    AttitudeProtocolProcess();
    ~AttitudeProtocolProcess();

    MspAttitudeDown attitude_down_;
    MspAttitudeDownDC attitude_dc_;

    void MspAttitudeDownToDC();

    virtual bool Pack(QByteArray &out_data) override;
    //virtual bool UnPack(BaseData *base_data, const std::vector<boost::uint8_t> in_packget);
    // 用于串口数据包的解包方法
    virtual bool UnPack(const QByteArray &in_data);
};

#endif // ATTITUDE_PROTOCOL_PROCESS_H
