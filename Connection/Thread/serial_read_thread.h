#ifndef SERIAL_READ_THREAD_H
#define SERIAL_READ_THREAD_H

#include <QThread>

class QSerialPort;

class SerialReadThread : public QThread {
  Q_OBJECT

public:
  explicit SerialReadThread(QObject *parent = 0, QSerialPort *serial = nullptr);
  void stop();
protected:
  void run() override;

public slots:
  void StartRead();

signals:
  void ResultReady(const QByteArray &read_data);

private:
  QSerialPort *serial_;
  volatile bool stopped_;
};

#endif // SERIAL_READ_THREAD_H
