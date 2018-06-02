#include "attitude_control.h"
#include "ui_attitudecontrol.h"

#include <QPainter>
#include <QTimer>

AttitudeControl::AttitudeControl(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::AttitudeControl),
  roll_angle_(0.0),
  pitch_angle_(0.0)
{
  ui->setupUi(this);

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &AttitudeControl::TimerUpdate);
  //timer_->start(100);
}

AttitudeControl::~AttitudeControl()
{
  delete ui;
}

void AttitudeControl::TimerUpdate() {
  roll_angle_ += 1;
  if (roll_angle_ > 90.0) {
    roll_angle_ = -90.0;
  }

  pitch_angle_ += 1;
  if (pitch_angle_ > 30.0) {
    pitch_angle_ = -30.0;
  }

  this->update();
}


void AttitudeControl::paintEvent(QPaintEvent *event) {
  // 初始化
  QPainter painter(this);

  // 设置黑色背景
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);
  painter.drawRect(rect());

  // 坐标转换对象
  QTransform transform;
  // 设置坐标原点为控件中心
  transform.translate(width() / 2, height() / 2);
  // 设置控件缩放，当窗口改变大小时，绘制的内容会变化大小
  int side = qMin(width(), height());
  transform.scale(side / 300.0, side / 300.0);
  painter.setWorldTransform(transform);
  painter.setRenderHint(QPainter::Antialiasing);

  // 绘制圆 + 刻度线
  DrawCircle(painter, transform);

  // 绘制中间静态刻度线 [-30, 30]
  DrawCenterScale(painter, transform);

  painter.end();
}

void AttitudeControl::DrawCircle(QPainter &painter, QTransform transform) {
  painter.save();
  QPen pen(Qt::white, 3);
  QBrush brush(Qt::red);
  painter.setPen(pen);

  painter.save();
  // rotate roll
  transform.rotate(roll_angle_);
  painter.setWorldTransform(transform);

  // draw bgcolor
  painter.setBrush(QColor(65, 105, 225));
  int start_angle = 0 * 16;
  int span_angle = 180 * 16;
  painter.drawPie(QRect(-140, -140, 280, 280), start_angle, span_angle);
  start_angle = 0 * 16;
  span_angle = -180 * 16;
  painter.setBrush(QColor(160, 82, 45));
  painter.drawPie(QRect(-140, -140, 280, 280), start_angle, span_angle);
  painter.restore();


  // draw top scale
  painter.save();
  painter.rotate(-30);
  int long_line = 20;
  int short_line = 15;
  for (int i = 0; i < 7; i++) {
    if (i == 3) {
      painter.drawLine(0, -140 + long_line, 0, -140);
    } else {
      painter.drawLine(0, -140 + short_line, 0, -140);
    }
    painter.rotate(10);
  }
  painter.restore();

  // draw left scale
  painter.save();
  painter.rotate(-90);
  painter.drawLine(0, -140 + long_line, 0, -140);
  painter.rotate(30);
  painter.drawLine(0, -140 + long_line, 0, -140);
  painter.restore();

  // draw right scale
  painter.save();
  painter.rotate(90);
  painter.drawLine(0, -140 + long_line, 0, -140);
  painter.rotate(-30);
  painter.drawLine(0, -140 + long_line, 0, -140);
  painter.restore();


  // draw roll scale
  painter.save();

  // rotate roll
  // don't need this.
  //transform.rotate(10);
  painter.setWorldTransform(transform);

  pen.setWidth(2);
  painter.setPen(pen);
  brush.setColor(Qt::red);
  painter.setBrush(brush);
  QPainterPath roll_path;
  roll_path.moveTo(0, -115);
  roll_path.lineTo(-8, -105);
  roll_path.lineTo(8, -105);
  roll_path.lineTo(0, -115);

  painter.drawPath(roll_path);

  painter.restore();

  painter.save();
  //draw roll value
  brush.setColor(Qt::transparent);
  painter.setBrush(brush);
  painter.setWorldTransform(transform);
  painter.drawRect(-12, -100, 24, 15);
  if (std::abs(roll_angle_) < 10) {
    painter.drawText(-6, -89, QString("0") + QString::number(std::abs(roll_angle_)));
  } else {
    painter.drawText(-6, -89, QString::number(std::abs(roll_angle_)));
  }

  painter.restore();

  painter.restore();
}

void AttitudeControl::DrawCenterScale(QPainter &painter, QTransform transform) {
  painter.save();


  transform.rotate(roll_angle_);
  transform.translate(0, pitch_angle_);

  /*
  if (pitch_angle_ != 0.0) {
    transform.translate(0, -pitch_angle_ - 10);
  } else {
    transform.translate(0, 0);
  }
  */

  painter.setWorldTransform(transform);

  QPen pen(Qt::white, 2);
  QFont font("楷体", 13);
  painter.setFont(font);
  painter.setPen(pen);

  int y_offset = -60;
  int line = 80;
  for (int i = -30; i < 40; i += 10) {
    painter.drawLine(-line / 2, y_offset, line / 2, y_offset);

    if (i != 0) {
      // 左边刻度值
      painter.drawText((-line / 2) - 22, y_offset + 3, QString::number(std::abs(i)));
      // 右边刻度值
      painter.drawText((line / 2) + 5, y_offset + 3, QString::number(std::abs(i)));
    }


    y_offset += 20;
    if (y_offset <= 0) {
      line -= 20;
    } else {
      line += 20;
    }
  }

  painter.restore();
}




void AttitudeControl::setRoll(const double roll_angle) {
  roll_angle_ = roll_angle;
  this->update();
}

void AttitudeControl::setPitch(const double pitch_angle) {
  pitch_angle_ = pitch_angle;
  this->update();
}

void AttitudeControl::UpdateView() {
  this->update();
}







