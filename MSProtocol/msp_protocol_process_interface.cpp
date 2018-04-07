#include "msp_protocol_process_interface.h"


MSProtocolProcessInterface::MSProtocolProcessInterface()
{

}


MSProtocolProcessInterface::~MSProtocolProcessInterface()
{

}

/***********************************************************************
* 函数名: GetMessageHeaderInfo
* 功能: 获取数据包的消息头
* 参数: 参数1 : 数据包引用
       参数2 : 存储数据包消息头的引用
* 返回值: 无
* 备注: suggest create msp_protocol_utils class, and write it to it
* 时间: 2018/3/23 陈登龙
***********************************************************************/
void MSProtocolProcessInterface::GetMessageHeaderInfo(const QByteArray &in_packget, MessageHeader &msg_header)
{
    msg_header.header_start = in_packget.at(0);
    msg_header.header_m = in_packget.at(1);
    msg_header.direction = in_packget.at(2);
    msg_header.size = in_packget.at(3);
    msg_header.command = in_packget.at(4);
}
