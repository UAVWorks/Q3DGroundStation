#include "tcp_client.h"

TcpClient::TcpClient(): pump_(), client_(pump_)
{

}


TcpClient::~TcpClient()
{
    if (pump_.is_running()) {
        pump_.stop_service();
    }
}


/*! @function
********************************************************
* 函数名: SetConnectAddr
* 功 能: 设置 TCP 的连接地址
* 参 数: port 连接的端口, ip 连接的ip
* 返回值: void
* 作 者: 陈登龙 南昌航空大学信息工程学院自动控制系
* 日 期: 2018-03-23
********************************************************/
void TcpClient::SetConnectAddr(unsigned short port, const std::string &ip)
{
    client_.set_server_addr(port, ip);
}

/*! @function
********************************************************
* 函数名: StartTcpConnect
* 功 能: 开启 TCP 连接
* 参 数: void
* 返回值: void
* 作 者: 陈登龙 南昌航空大学信息工程学院自动控制系
* 日 期: 2018-03-23
********************************************************/
void TcpClient::StartTcpConnect()
{
    pump_.start_service();
}

/*! @function
********************************************************
* 函数名:StopTcpConnect
* 功 能: 停止 TCP 连接
* 参 数: void
* 返回值:void
* 作 者: 陈登龙 南昌航空大学信息工程学院自动控制系
* 日 期: 2018-03-23
********************************************************/
void TcpClient::StopTcpConnect()
{
    if (pump_.is_running()) {
        pump_.stop();
        pump_.stop_service();
    }
}

/*! @function
********************************************************
* 函数名: IsTcpConnecting
* 功 能: 判断 TCP 当前是否已经连接
* 参 数: void
* 返回值: 如果 TCP 已经连接返回 true，否则返回 false
* 作 者: 陈登龙 南昌航空大学信息工程学院自动控制系
* 日 期: 2018-03-23
********************************************************/
bool TcpClient::IsTcpConnecting()
{
    return client_.is_connected();
}
