#ifndef MSP_PROTOCOL_PROCESS_INTERFACE_H
#define MSP_PROTOCOL_PROCESS_INTERFACE_H
/***************************************************************
* Copyright (c) 2018, 陈登龙
* All rights reserved.
*
* 文件名称：map_protocol_process_interface.h
* 摘   要：MSP 协议打包解包算法接口
*
* 当前版本：1.0
* 作   者：陈登龙
* 完成日期：2018-03-21
***************************************************************/

#include "msp_protocol_structs.h"

#include <vector>
#include <QByteArray>

class MSProtocolProcessInterface
{
public:
    MSProtocolProcessInterface();
    virtual ~MSProtocolProcessInterface();

    static void GetMessageHeaderInfo(const QByteArray &in_packget, MessageHeader &msg_header);

    virtual bool Pack(QByteArray &out_data) = 0;
    //virtual bool UnPack(BaseData *base_data, const std::vector<boost::uint8_t> in_packget) = 0;
    virtual bool UnPack(const QByteArray &in_data) = 0;
};


#endif // MSP_PROTOCOL_PROCESS_INTERFACE_H
