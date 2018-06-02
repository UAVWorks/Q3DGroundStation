#include "serial_read_thread.h"

#include <QSerialPort>

#include <common/mavlink.h>

SerialReadThread::SerialReadThread(QObject *parent, QSerialPort *serial):
  QThread(parent),
  serial_(serial),
  stopped_(false)
{

}

void SerialReadThread::run() {
  if (!stopped_) {
    QByteArray read_data = serial_->readAll();

    emit ResultReady(read_data);

    stopped_ = false;
  }
#if 0
  mavlink_message_t msg;
  mavlink_status_t status;

  for (int i = 0; i < read_data.length(); i++) {
    if (mavlink_parse_char(MAVLINK_COMM_0, read_data[i], &msg, &status)) {
      switch (msg.msgid) {
        // Heartbeat - ok
        case MAVLINK_MSG_ID_HEARTBEAT:
          mavlink_heartbeat_t heartbeat;
          mavlink_msg_heartbeat_decode(&msg, &heartbeat);
          break;
      }
    }
  }
#endif
}

void SerialReadThread::StartRead() {
  this->start();
}



void SerialReadThread::stop() {
  stopped_ = true;
}


