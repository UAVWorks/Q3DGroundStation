#include "serial_write_thread.h"


WriteSerialController::WriteSerialController(QSerialPort *serial) {
  WriteSerialWorker *write_worker = new WriteSerialWorker(serial);
  write_worker->moveToThread(&write_thread);

  connect(this, &WriteSerialController::WriteMessage, write_worker, &WriteSerialWorker::StartWork);
  connect(write_worker, &WriteSerialWorker::WriteComplete, this, &WriteSerialController::HandleComplete);
  connect(&write_thread, &QThread::finished, write_worker, &QObject::deleteLater);
  write_thread.start();
}

WriteSerialController::~WriteSerialController() {
  write_thread.quit();
  write_thread.wait();
}


void WriteSerialController::HandleComplete() {



}
