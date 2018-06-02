#include "serial_write_thread.h"

#include <QSerialPort>

SerialWriteThread::SerialWriteThread(QObject *parent, QSerialPort *serial):
  QThread(parent),
  serial_(serial),
  stopped_(false)
{
  write_data_.clear();
}


void SerialWriteThread::run() {
  if (!stopped_) {
    serial_->write(write_data_);
    stopped_ = false;
  }
}

void SerialWriteThread::WriteMessage(const QByteArray &write_data) {
  write_data_ = write_data;
}

void SerialWriteThread::stop() {
  stopped_ = true;
}
