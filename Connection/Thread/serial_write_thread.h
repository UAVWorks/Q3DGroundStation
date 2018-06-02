#ifndef SERIAL_WRITE_THREAD_H
#define SERIAL_WRITE_THREAD_H

#include <QThread>

class QSerialPort;

class SerialWriteThread : public QThread {
public:
  explicit SerialWriteThread(QObject *parent = 0, QSerialPort *serial = nullptr);
  void stop();
protected:
  void run() override;

public slots:
  void WriteMessage(const QByteArray &write_data);

private:
  QSerialPort *serial_;
  QByteArray write_data_;
  volatile bool stopped_;
};

#endif // SERIAL_WRITE_THREAD_H
