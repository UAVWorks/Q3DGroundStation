#include "flight_control.h"
#include "ui_flightcontrol.h"
#include "q3dgs.h"

#include <QPainter>
#include <QKeyEvent>


#include "cmd_param.h"
#include "track_planning.h"

FlightControl::FlightControl(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FlightControl),
  cmd_param_(nullptr),
  trask_planning_(nullptr)
{
  ui->setupUi(this);
  memset((void*)(&cmd_packet_), 0, sizeof(mavlink_command_long_t));
  memset((void*)(&rc_override_), 0, sizeof(mavlink_rc_channels_override_t));
  memset((void*)(&set_mode_), 0, sizeof(mavlink_set_mode_t));

  rc_override_.target_system = kTargetSystemId;
  rc_override_.target_component = kTargetComponentId;
  //rc_override_.chan1_raw = 1000;
  //rc_override_.chan2_raw = 1000;
  //rc_override_.chan3_raw = 1000;
  //rc_override_.chan4_raw = 1000;

  cmd_param_ = new CmdParam;
  cmd_param_->hide();

  trask_planning_ = new TrackPlanning;
  trask_planning_->hide();

  connect(cmd_param_, &CmdParam::SendCmdParams, this, &FlightControl::ReceiveCmdParams);
}

FlightControl::~FlightControl()
{
  delete ui;
}


void FlightControl::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);
  painter.drawRect(rect());
  painter.end();
}


/**
 * @brief unlock
 *
 * throttle: chan3_raw
 * yaw: chan4_raw
 * un_lock: chan3_raw min + chan4_raw max + 5s
 * lock: chan3_raw min + chan4_raw min
 *
 * 800 - 2200
 */
void FlightControl::on_unlock_clicked() {
  memset((void*)(&rc_override_), 0, sizeof(mavlink_rc_channels_override_t));
  rc_override_.target_system = kTargetSystemId;
  rc_override_.target_component = kTargetComponentId;

  if (ui->unlock->text() == QObject::tr("Unlock")) {
    rc_override_.chan3_raw = 0;//1015
    rc_override_.chan4_raw = 2200;//1996
    // send unlock signal
    emit SendUnlockSignal(&rc_override_);
    ui->unlock->setText(QObject::tr("Lock"));
  } else {
    rc_override_.chan3_raw = 1000;
    rc_override_.chan4_raw = 1000;

    // send lock signal
    emit SendUnlockSignal(&rc_override_);
    ui->unlock->setText(QObject::tr("Unlock"));
  }
}

#if 0
/**
 * @brief send unlock cmd signal to Connection
 */
void FlightControl::on_unlock_clicked() {
  memset((void*)(&cmd_packet_), 0, sizeof(mavlink_command_long_t));
  cmd_packet_.command = MAV_CMD_COMPONENT_ARM_DISARM;
  cmd_packet_.target_system = kTargetSystemId;
  cmd_packet_.target_component = kTargetComponentId;

  if (ui->unlock->text() == QObject::tr("Unlock")) {
    // pack unlock cmd packet
    // 1: arm, 0: disarm
    cmd_packet_.param1 = 1;

    // send unlock signal
    emit SendMAVLinkCmd(&cmd_packet_);
    ui->unlock->setText(QObject::tr("Lock"));
  } else {
    // 1: arm, 0: disarm
    cmd_packet_.param1 = 0;

    // send lock signal
    emit SendMAVLinkCmd(&cmd_packet_);
    ui->unlock->setText(QObject::tr("Unlock"));
  }
}
#endif


/**
 * @brief Send LOT cmd
 *
 * @note Command ACCEPTED and EXECUTED
 */
void FlightControl::on_loiter_clicked() {
  memset((void*)(&cmd_packet_), 0, sizeof(mavlink_command_long_t));
  cmd_packet_.command = MAV_CMD_NAV_LOITER_UNLIM;
  cmd_packet_.target_system = kTargetSystemId;
  cmd_packet_.target_component = kTargetComponentId;

  // Update cmd dialog label.
  cmd_param_->SetParam1Label(QString("Empty"));
  cmd_param_->SetParam2Label(QString("Empty"));
  cmd_param_->SetParam3Label(QString("Radius around waypoint"));
  cmd_param_->SetParam4Label(QString("Yaw angle"));
  cmd_param_->SetParam5Label(QString("lat"));
  cmd_param_->SetParam6Label(QString("lon"));
  cmd_param_->SetParam7Label(QString("alt"));

  // show model dialog
  if (cmd_param_->exec() == QDialog::Accepted) {
    // set param to cmd packet
    cmd_packet_.param1 = params_.param1;
    cmd_packet_.param2 = params_.param2;
    cmd_packet_.param3 = params_.param3;
    cmd_packet_.param4 = params_.param4;
    cmd_packet_.param5 = params_.param5;
    cmd_packet_.param6 = params_.param6;
    cmd_packet_.param7 = params_.param7;
    emit SendMAVLinkCmd(&cmd_packet_);
  } else {
    // do nothing.
  }
}


/**
 * @brief Send LOT cmd
 *
 * @note Command ACCEPTED and EXECUTED
 */
void FlightControl::on_rtl_clicked() {
  memset((void*)(&cmd_packet_), 0, sizeof(mavlink_command_long_t));
  cmd_packet_.command = MAV_CMD_NAV_RETURN_TO_LAUNCH;
  cmd_packet_.target_system = kTargetSystemId;
  cmd_packet_.target_component = kTargetComponentId;

  // This cmd no params.
  emit SendMAVLinkCmd(&cmd_packet_);
}


/**
 * @brief Send LOT cmd
 *
 * @note Command ACCEPTED and EXECUTED
 */
void FlightControl::on_land_clicked() {
  memset((void*)(&cmd_packet_), 0, sizeof(mavlink_command_long_t));
  cmd_packet_.command = MAV_CMD_NAV_LAND;
  cmd_packet_.target_system = kTargetSystemId;
  cmd_packet_.target_component = kTargetComponentId;

  // Update cmd dialog label.
  cmd_param_->SetParam1Label(QString("Abort Alt"));
  cmd_param_->SetParam2Label(QString("Precision land mode"));
  cmd_param_->SetParam3Label(QString("Empty"));
  cmd_param_->SetParam4Label(QString("Desired yaw angle"));
  cmd_param_->SetParam5Label(QString("lat"));
  cmd_param_->SetParam6Label(QString("lon"));
  cmd_param_->SetParam7Label(QString("alt"));

  // show model dialog
  if (cmd_param_->exec() == QDialog::Accepted) {
    // set param to cmd packet
    cmd_packet_.param1 = params_.param1;
    cmd_packet_.param2 = params_.param2;
    cmd_packet_.param3 = params_.param3;
    cmd_packet_.param4 = params_.param4;
    cmd_packet_.param5 = params_.param5;
    cmd_packet_.param6 = params_.param6;
    cmd_packet_.param7 = params_.param7;
    emit SendMAVLinkCmd(&cmd_packet_);
  } else {
    // do nothing.
  }
}

#if 0
/**
 * @brief Send LOT cmd
 *
 * @note This APM don't sup this cmd.
 */
void FlightControl::on_alt_clicked() {
  memset((void*)(&cmd_packet_), 0, sizeof(mavlink_command_long_t));
  cmd_packet_.command = MAV_CMD_NAV_LOITER_TO_ALT;
  cmd_packet_.target_system = kSystemId;
  cmd_packet_.target_component = kComponentId;

  // Update cmd dialog label.
  cmd_param_->SetParam1Label(QString("Heading Required"));
  cmd_param_->SetParam2Label(QString("Radius in meters"));
  cmd_param_->SetParam3Label(QString("Empty"));
  cmd_param_->SetParam4Label(QString("Forward"));
  cmd_param_->SetParam5Label(QString("lat"));
  cmd_param_->SetParam6Label(QString("lon"));
  cmd_param_->SetParam7Label(QString("alt"));

  // show model dialog
  if (cmd_param_->exec() == QDialog::Accepted) {
    // set param to cmd packet
    cmd_packet_.param1 = params_.param1;
    cmd_packet_.param2 = params_.param2;
    cmd_packet_.param3 = params_.param3;
    cmd_packet_.param4 = params_.param4;
    cmd_packet_.param5 = params_.param5;
    cmd_packet_.param6 = params_.param6;
    cmd_packet_.param7 = params_.param7;
    emit SendMAVLinkCmd(&cmd_packet_);
  } else {
    // do nothing.
  }
}
#endif

#if 0
/**
 * @brief Send SET_MODE cmd
 *
 * @note This APM don't sup this cmd.
 */
void FlightControl::on_set_mode_clicked() {
  memset((void*)(&cmd_packet_), 0, sizeof(mavlink_command_long_t));
  cmd_packet_.command = MAV_CMD_DO_SET_MODE;
  cmd_packet_.target_system = kSystemId;
  cmd_packet_.target_component = kComponentId;

  // Update cmd dialog label.
  cmd_param_->SetParam1Label(QString("Mode"));
  cmd_param_->SetParam2Label(QString("Custom mode"));
  cmd_param_->SetParam3Label(QString("Custom sub mode"));
  cmd_param_->SetParam4Label(QString("Empty"));
  cmd_param_->SetParam5Label(QString("Empty"));
  cmd_param_->SetParam6Label(QString("Empty"));
  cmd_param_->SetParam7Label(QString("Empty"));

  // show model dialog
  if (cmd_param_->exec() == QDialog::Accepted) {
    // set param to cmd packet
    cmd_packet_.param1 = params_.param1;
    cmd_packet_.param2 = params_.param2;
    cmd_packet_.param3 = params_.param3;
    cmd_packet_.param4 = params_.param4;
    cmd_packet_.param5 = params_.param5;
    cmd_packet_.param6 = params_.param6;
    cmd_packet_.param7 = params_.param7;
    emit SendMAVLinkCmd(&cmd_packet_);
  } else {
    // do nothing.
  }
}

#endif


#if 0
/**
 * @brief Send LOT cmd
 *
 * @note This APM don't sup this cmd.
 */
void FlightControl::on_set_home_clicked() {
  memset((void*)(&cmd_packet_), 0, sizeof(mavlink_command_long_t));
  cmd_packet_.command = MAV_CMD_DO_SET_HOME;
  cmd_packet_.target_system = kSystemId;
  cmd_packet_.target_component = kComponentId;

  // Update cmd dialog label.
  cmd_param_->SetParam1Label(QString("Use current"));
  cmd_param_->SetParam2Label(QString("Empty"));
  cmd_param_->SetParam3Label(QString("Empty"));
  cmd_param_->SetParam4Label(QString("Empty"));
  cmd_param_->SetParam5Label(QString("lat"));
  cmd_param_->SetParam6Label(QString("lon"));
  cmd_param_->SetParam7Label(QString("alt"));

  // show model dialog
  if (cmd_param_->exec() == QDialog::Accepted) {
    // set param to cmd packet
    cmd_packet_.param1 = params_.param1;
    cmd_packet_.param2 = params_.param2;
    cmd_packet_.param3 = params_.param3;
    cmd_packet_.param4 = params_.param4;
    cmd_packet_.param5 = params_.param5;
    cmd_packet_.param6 = params_.param6;
    cmd_packet_.param7 = params_.param7;
    emit SendMAVLinkCmd(&cmd_packet_);
  } else {
    // do nothing.
  }
}

#endif

#if 0
void FlightControl::on_mission_start_clicked() {
  memset((void*)(&cmd_packet_), 0, sizeof(mavlink_command_long_t));
  cmd_packet_.command = MAV_CMD_MISSION_START;
  cmd_packet_.target_system = kSystemId;
  cmd_packet_.target_component = kComponentId;

  // Update cmd dialog label.
  cmd_param_->SetParam1Label(QString("first_item"));
  cmd_param_->SetParam2Label(QString("last_item"));
  cmd_param_->SetParam3Label(QString("Empty"));
  cmd_param_->SetParam4Label(QString("Empty"));
  cmd_param_->SetParam5Label(QString("Empty"));
  cmd_param_->SetParam6Label(QString("Empty"));
  cmd_param_->SetParam7Label(QString("Empty"));

  // show model dialog
  if (cmd_param_->exec() == QDialog::Accepted) {
    // set param to cmd packet
    cmd_packet_.param1 = params_.param1;
    cmd_packet_.param2 = params_.param2;
    cmd_packet_.param3 = params_.param3;
    cmd_packet_.param4 = params_.param4;
    cmd_packet_.param5 = params_.param5;
    cmd_packet_.param6 = params_.param6;
    cmd_packet_.param7 = params_.param7;
    emit SendMAVLinkCmd(&cmd_packet_);
  } else {
    // do nothing.
  }
}
#endif


#if 0
void FlightControl::on_mavlink_version_clicked() {
  memset((void*)(&cmd_packet_), 0, sizeof(mavlink_command_long_t));
  cmd_packet_.command = MAV_CMD_REQUEST_PROTOCOL_VERSION;
  cmd_packet_.target_system = kSystemId;
  cmd_packet_.target_component = kComponentId;

  // Update cmd dialog label.
  cmd_param_->SetParam1Label(QString("Request supported protocol versions"));
  cmd_param_->SetParam2Label(QString("Reserved(all remaining params)"));
  cmd_param_->SetParam3Label(QString("Empty"));
  cmd_param_->SetParam4Label(QString("Empty"));
  cmd_param_->SetParam5Label(QString("Empty"));
  cmd_param_->SetParam6Label(QString("Empty"));
  cmd_param_->SetParam7Label(QString("Empty"));

  // show model dialog
  if (cmd_param_->exec() == QDialog::Accepted) {
    // set param to cmd packet
    cmd_packet_.param1 = params_.param1;
    cmd_packet_.param2 = params_.param2;
    cmd_packet_.param3 = params_.param3;
    cmd_packet_.param4 = params_.param4;
    cmd_packet_.param5 = params_.param5;
    cmd_packet_.param6 = params_.param6;
    cmd_packet_.param7 = params_.param7;
    emit SendMAVLinkCmd(&cmd_packet_);
  } else {
    // do nothing.
  }
}
#endif


void FlightControl::ReceiveCmdParams(struct Params &params) {
  params_ = params;
}


void FlightControl::on_mission_clicked() {
  // trask planning
  trask_planning_->show();
}


/**
 * @brief start or stop key ctl
 *
 * roll: chan1_raw
 * pitch: chan2_raw
 * throttle: chan3_raw
 * yaw: chan4_raw
 */
void FlightControl::on_key_ctl_clicked() {
  if (ui->key_ctl->text() == QString("KeyCtl")) {
    setFocus();
    ui->key_ctl->setText(QString("KeyNoCtl"));
  } else {
    setFocusPolicy(Qt::NoFocus);
    ui->key_ctl->setText(QString("KeyCtl"));
  }
}


void FlightControl::keyPressEvent(QKeyEvent *event) {
  std::lock_guard<std::mutex> lock(mutex_);

  auto key = event->key();
  int inc = 100;
  if (key == Qt::Key_W) {
    // ¡ü throttle: ¡ü r p t y
    rc_override_.chan1_raw += inc;
    rc_override_.chan2_raw += inc;
    rc_override_.chan3_raw += inc;
    rc_override_.chan4_raw += inc;
  } else if (key == Qt::Key_S) {
    // ¡ý throttle: ¡ý r p t y
    rc_override_.chan1_raw -= inc;
    rc_override_.chan2_raw -= inc;
    rc_override_.chan3_raw -= inc;
    rc_override_.chan4_raw -= inc;
    rc_override_.chan3_raw -= inc;
  } else if (key == Qt::Key_A) {
    // ¡û yaw: ¡ü t y
    rc_override_.chan3_raw += inc;
    rc_override_.chan4_raw += inc;
  } else if (key == Qt::Key_D) {
    // ¡ú yaw: ¡ü r p
    rc_override_.chan1_raw += inc;
    rc_override_.chan2_raw += inc;
  } else if (key == Qt::Key_Up) {
    // ¡ü pitch: ¡ü p y
    rc_override_.chan2_raw += inc;
    rc_override_.chan4_raw += inc;
  } else if (key == Qt::Key_Down) {
    // ¡ý pitch: ¡ü r t
    rc_override_.chan1_raw += inc;
    rc_override_.chan3_raw += inc;
  } else if (key == Qt::Key_Left) {
    // ¡û roll: ¡ü r y
    rc_override_.chan1_raw += inc;
    rc_override_.chan4_raw += inc;
  } else if (key == Qt::Key_Right) {
    // ¡ú roll: ¡ü p t
    rc_override_.chan2_raw += inc;
    rc_override_.chan3_raw += inc;
  } else {
    QWidget::keyPressEvent(event);
  }

  emit SendRcChannels(&rc_override_);
}


void FlightControl::keyReleaseEvent(QKeyEvent *event) {
  //emit SendRcChannels(&rc_override_);
}



void FlightControl::UpdateServoOutput(const mavlink_servo_output_raw_t &servo_out) {
  rc_override_.chan1_raw = servo_out.servo1_raw;
  rc_override_.chan2_raw = servo_out.servo2_raw;
  rc_override_.chan3_raw = servo_out.servo3_raw;
  rc_override_.chan4_raw = servo_out.servo4_raw;
}


void FlightControl::on_set_mode_clicked() {
  memset((void*)(&set_mode_), 0, sizeof(mavlink_set_mode_t));
  // set_mode_.base_mode has CUSTOM_MODE_FALG
  // COPTER_MODE_GUIDED = 4
  set_mode_.custom_mode = 4;
  set_mode_.target_system = kTargetSystemId;
}





void FlightControl::on_take_off_clicked()
{

}
