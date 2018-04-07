#include "attitude_protocol_process.h"

#include <QDebug>

#include <boost/scoped_ptr.hpp>

AttitudeProtocolProcess::AttitudeProtocolProcess()
{

}


AttitudeProtocolProcess::~AttitudeProtocolProcess()
{

}

/***************************************************************
* 函数名: Pack
* 功能: 用于串口上行数据包的打包方法
* 参数: 串口上行数据包引用
* 返回值: 打包成功:true，打包失败:false
* 备注: 将参数换成 QByteArray 类型
* 时间: 2018/3/23 陈登龙
*****************************************************************/
bool AttitudeProtocolProcess::Pack(QByteArray &out_data)
{
  out_data.clear();

  // set out_data MSP_VERSION
  out_data.append(kHeaderStart);
  out_data.append(kHeaderM);
  out_data.append(kGcsToUav);
  // size = 0
  out_data.append(static_cast<char>(0));
  // cmd = 108
  out_data.append(static_cast<char>(MSP_ATTITUDE));
  // crc = 0 ^ 108
  out_data.append(static_cast<char>(0 ^ MSP_ATTITUDE));

  /*
    // 分配上行姿态数据包
    MspAttitudeUp msp_attitude_up;
    msp_attitude_up.message_header.header_start = kHeaderStart;
    msp_attitude_up.message_header.header_m = kHeaderM;
    msp_attitude_up.message_header.direction = kGcsToUav;
    msp_attitude_up.message_header.size = 0;
    msp_attitude_up.message_header.command = MSP_ATTITUDE;
    msp_attitude_up.crc = msp_attitude_up.message_header.size ^ msp_attitude_up.message_header.command;

    int length = sizeof(struct MspAttitudeUp);
    unsigned char* tmp = new unsigned char[length];
    memcpy(tmp, &msp_attitude_up, length);

    // 清空待发送字节数组
    out_packget.clear();
    // 拷贝到待发送字节数组
    for (int i = 0; i < length; i++)
    {
        out_packget.push_back(*(tmp + i));
    }

    delete tmp;

    if (!out_packget.empty())
    {
        return true;
    }
    else
    {
        return false;
    }
    */

  return true;
}

/*
bool AttitudeProtocolProcess::UnPack(BaseData *base_data, const std::vector<boost::uint8_t> in_packget)
{

    return true;
}
*/

/***************************************************************
* 函数名: UnPack
* 功能: 用于串口下行数据包的解包方法
* 参数: 串口下行数据包引用
* 返回值: 解包成功:true，解包失败:false
* 备注: 该函数有另一个重载版本
* 时间: 2018/3/23 陈登龙
*****************************************************************/
bool AttitudeProtocolProcess::UnPack(const QByteArray &in_data)
{
  // unpack header
  GetMessageHeaderInfo(in_data, attitude_down_.message_header);

  // msg header is 5 byte
  int headr_len = sizeof(struct MessageHeader);
  int data_start_index = headr_len;

  // unpack data. NOTE: LSB
  attitude_down_.roll = in_data.at(data_start_index) | (in_data.at(data_start_index + 1) << 8);
  attitude_down_.pitch = in_data.at(data_start_index + 2) | (in_data.at(data_start_index + 3) << 8);
  attitude_down_.yaw = in_data.at(data_start_index + 4) | (in_data.at(data_start_index + 5) << 8);
  attitude_down_.crc = in_data.at(data_start_index + 6);

  return true;
}

/***************************************************************
* 函数名: MspAttitudeDownToDC
* 功能: Convert MspAttitude data packget to real data.
* 参数: void
* 返回值: void
* 备注:
* 时间: 2018/3/23 陈登龙
*****************************************************************/
void AttitudeProtocolProcess::MspAttitudeDownToDC() {
    attitude_dc_.message_header = attitude_down_.message_header;
    attitude_dc_.roll = static_cast<double>(attitude_down_.roll) / 10.0;
    attitude_dc_.pitch = static_cast<double>(attitude_down_.pitch) / 10.0;
    attitude_dc_.yaw = static_cast<double>(attitude_down_.yaw) / 10.0;
    attitude_dc_.crc = attitude_down_.crc;
}








