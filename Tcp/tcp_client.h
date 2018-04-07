#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
/***************************************************************
* Copyright (c) 2018, 陈登龙
* All rights reserved.
*
* 文件名称：TcpClient.h
* 摘    要：用于本项目的 TCP 通信类。
*
* 当前版本：1.0
* 作    者：陈登龙
* 完成日期：2018-03-23
***************************************************************/

#define SERVER_PORT    9527
#define SERVER_IP      "127.10.10.16"
#undef  MAX_MSG_NUM
#define MAX_MSG_NUM	   1500
#define FORCE_TO_USE_MSG_RECV_BUFFER  //force to use the msg recv buffer

#include <string>

#include "../Communication/st_asio_wrapper_base.h"
#include "../Communication/st_asio_wrapper_tcp_client.h"
#include "../MSProtocol/attitude_protocol_process.h"
//#include "../MessageBus/MessageBus.hpp"

using namespace st_asio_wrapper;

//extern MessageBus g_bus;

/*! @class
*******************************************************************************
* 类名称 : TcpClient
* 功   能: TcpClient 提供基于 Boost.Asio 完全异步加多线程的网络通信接口。
* 作   者: 陈登龙 南昌航空大学信息工程学院自动控制系
* 当前版本: 1.0
* 作   者: 陈登龙
* 完成日期: 2018-03-23
*******************************************************************************/
class TcpClient
{
public:
    TcpClient();
    ~TcpClient();

public:
    // 设置 TCP 的连接地址
    void SetConnectAddr(unsigned short port, const std::string& ip);

    // 开启 TCP 连接
    void StartTcpConnect();

    // 停止 TCP 连接
    void StopTcpConnect();

    // 判断 TCP 当前是否已经连接
    bool IsTcpConnecting();
private:
    /*! @class
    *******************************************************************************
    * 类名称 : MyConnector
    * 功   能: MyConnector 是基于 Boost.Asio 的 st_connector 拓展的 tcp-socket，
              作为类型传入 st_sclient<MyConnector> m_client,作为本项目的通信客户端对象。
    * 作   者: 陈登龙 南昌航空大学信息工程学院自动控制系
    * 当前版本: 1.0
    * 作   者: 陈登龙
    * 完成日期: 2016-10-17
    *******************************************************************************/
    class MyConnector : public st_connector
    {
        public:
            MyConnector(boost::asio::io_service& io_service_) : st_connector(io_service_)
            {
                attitude_process_ = new AttitudeProtocolProcess;
            }
            ~MyConnector()
            {
                delete attitude_process_;
            }

        protected:
            typedef std::string MsgType;
            /*! @function
            ********************************************************************************
            * 函数名:  on_msg_handle
            * 功  能: 接收通过 TCP 协议发送到本终端的数据包
            * 参  数: msg 接收到的二进制 std::string 格式的数据包
            * 返回值:  void
            * 作  者: 陈登龙 南昌航空大学信息工程学院自动控制系
            * 日  期: 2018-03-24
            *******************************************************************************/
            virtual void on_msg_handle(MsgType &msg)
            {
                // 将 std::string -> QByteArray
                QByteArray byte_msg;

                for (int i = 0; i < msg.size(); i++) {
                    byte_msg.append(msg.at(i));
                }

                // 得到消息头
                //AttitudeProtocolProcess::GetMessageHeaderInfo(byte_msg, attitude_process_->msg_header_);

                // 解包得到消息体
                attitude_process_->UnPack(byte_msg);

                // 转换为物理数据结构体
                attitude_process_->MspAttitudeDownToDC();

                qDebug("roll: %f", attitude_process_->attitude_dc_.roll);
                qDebug("pitch: %f", attitude_process_->attitude_dc_.pitch);
                qDebug("yaw: %f", attitude_process_->attitude_dc_.yaw);

                // 将物理数据结构体发送到消息总线上
                //g_bus.SendReq<void, const MspAttitudeDownDC&>(attitude_process_->attitude_dc_, "MspAttitudeDownDC");
            }
        private:
            AttitudeProtocolProcess *attitude_process_;

    };

private:
    // TCP 客户端服务泵
    st_service_pump pump_;

    // TCP 通信的客户端对象
    st_sclient<MyConnector> client_;
};

#endif // TCP_CLIENT_H
