#ifndef SERIAL_READ_THREAD_H
#define SERIAL_READ_THREAD_H

#include <QThread>
#include <QSerialPort>
#include <queue>
#include <QTimer>

#include <common/mavlink.h>

#define PARSE_MSG_IN_THREAD 1

class ReadSerialWorker : public QObject {
  Q_OBJECT

public:
  ReadSerialWorker(QSerialPort *serial) :
    serial_(serial),
    timer_(new QTimer(this)),
    msg_length_(0)
  {
    qRegisterMetaType<mavlink_message_t>("mavlink_message_t");
    //connect(timer_, &QTimer::timeout, this, &ReadSerialWorker::TimerUpdate);
    //timer_->start(10);
  }

public slots:
  /**
   * @brief Start read serial data and parse it
   * @note usb: This 2 way is almost equal, need to unlock APM
   *     Telem: This 2 way is almost delay, need to change telem and ...
   */
  void StartWork() {
#if PARSE_MSG_IN_THREAD
    raw_msg_ = serial_->readAll();
    msg_length_ = raw_msg_.length();

    // need to parse msg
    for (int i = 0; i < msg_length_; i++) {
      if (mavlink_parse_char(MAVLINK_COMM_0, raw_msg_[i], &msg_, &status_)) {
        // push to msg buf
        //msg_buf_.push(msg_);
        emit MessageReady(msg_);
      }
    }
#else
    raw_msg_ = serial_->readAll();
    emit MessageReady(raw_msg_);
#endif
    /*
    // read one byte
    char read_byte = 0;
    while (serial_->bytesAvailable() > 0) {
      if (serial_->read(&read_byte, 1) > 0) {
        if (mavlink_parse_char(MAVLINK_COMM_0, read_byte, &msg_, &status_)) {
          // push to msg buf
          //msg_buf_.push(msg_);
          emit MessageReady(msg_);
        }
      }
    }
    */
  }
private slots:
  /*
  void TimerUpdate() {
    if (!msg_buf_.empty()) {
      emit MessageReady(msg_buf_.front());
      msg_buf_.pop();
    }
  }
  */

signals:
#if PARSE_MSG_IN_THREAD
  void MessageReady(const mavlink_message_t &read_msg);
#else
  void MessageReady(const QByteArray &raw_msg);
#endif


private:
  QSerialPort *serial_;
  std::queue<mavlink_message_t> msg_buf_;
  QTimer *timer_;
  mavlink_message_t msg_;
  mavlink_status_t status_;
  QByteArray raw_msg_;
  int msg_length_;
};

class ReadSerialController : public QObject {
  Q_OBJECT

public:
  ReadSerialController(QSerialPort *serial);
  ~ReadSerialController();

public slots:
#if PARSE_MSG_IN_THREAD
  void HandleMessage(const mavlink_message_t &read_msg);
#else
  void HandleMessage(const QByteArray &raw_msg);
#endif

signals:
  void UpdateUavInfo(const mavlink_heartbeat_t &heartbeat);
  void UpdateAttitude(const mavlink_attitude_t &attitude);
  void UpdateBatteryStatus(const mavlink_battery_status_t &battery);
  void UpdateSysStatusSensor(const mavlink_sys_status_t &sys_status);
  void UpdateGlobalPositionInt(const mavlink_global_position_int_t &global_position);
  void UpdateHUD(const mavlink_vfr_hud_t &vfr_hud);
  void UpdateServoOutput(const mavlink_servo_output_raw_t &servo_out);

private:
  QThread read_thread;
};





#endif // SERIAL_READ_THREAD_H
