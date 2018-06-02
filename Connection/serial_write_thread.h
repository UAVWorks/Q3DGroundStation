#ifndef SERIAL_WRITE_THREAD_H
#define SERIAL_WRITE_THREAD_H

#include <QThread>
#include <QSerialPort>

class WriteSerialWorker : public QObject {
  Q_OBJECT

public:
  WriteSerialWorker(QSerialPort *serial) : serial_(serial) {

  }

public slots:
  void StartWork(const QByteArray &write_msg) {
    serial_->write(write_msg);

    // get write status then to send signal.

    emit WriteComplete();
  }

signals:
  void WriteComplete();

private:
  QSerialPort *serial_;
};


class WriteSerialController : public QObject {
  Q_OBJECT

public:
  WriteSerialController(QSerialPort *serial);
  ~WriteSerialController();

signals:
  void WriteMessage(const QByteArray &write_msg);

private slots:
  void HandleComplete();

private:
  QThread write_thread;
};












#endif // SERIAL_WRITE_THREAD_H
