#include "hud_view.h"
#include "ui_hudview.h"

#include <QPainter>
#include <QTimer>

HUDView::HUDView(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::HUDView)
{
  ui->setupUi(this);

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &HUDView::TimerUpdate);
  //timer_->start(100);

  /**
   * @note every value need calibration
   */
  roll_angle_ = 0.0;
  pitch_angle_ = 0.0;
  yaw_angle_ = 0.0;
  speed_ = 0.0;
  altitude_ = 0.0;
  gps_status_ = true;
  battery_remain_ = 0;
}

HUDView::~HUDView()
{
  delete ui;
}

void HUDView::TimerUpdate() {

  battery_remain_ += 1;
  if (battery_remain_ > 100) {
    battery_remain_ = 0;
  }

  if (gps_status_) {
    gps_status_ = false;
  } else {
    gps_status_ = true;
  }

  roll_angle_ += 1;
  if (roll_angle_ == 60) {
    roll_angle_ = -60;
  }

  pitch_angle_ += 1;
  if (pitch_angle_ == 90) {
    pitch_angle_ = -90;
  }

  yaw_angle_ += 1;
  if (yaw_angle_ == 180) {
    yaw_angle_ = -180;
  }

  speed_ -= 1;
  if (speed_ < 0) {
    speed_ = 100;
  }

  altitude_ += 1;
  if (altitude_ > 100) {
    altitude_ = 0.0;
  }

  this->update();
}

void HUDView::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  // 坐标转换对象
  QTransform transform;
  // 设置坐标原点为控件中心
  transform.translate(width() / 2, height() / 2);
  // 设置控件缩放，当窗口改变大小时，绘制的内容会变化大小
  int side = qMin(width(), height());
  transform.scale(side / 300.0, side / 300.0);
  painter.setWorldTransform(transform);

  painter.setRenderHint(QPainter::Antialiasing);

  // 绘制蓝-黄背景色
  DrawHudBgColor(painter, transform);
  // 绘制横滚弧度
  DrawHudRollRadian(painter, transform);
  // 绘制俯仰刻度
  DrawHudPitchScale(painter, transform);
  // 绘制十字镐
  DrawHudCrossbow(painter, transform);
  // 绘制偏航刻度
  DrawHudYawScale(painter, transform);
  // 绘制速度
  DrawHudSpeed(painter, transform);
  // 绘制高度
  DrawHudAltitude(painter, transform);
  // 绘制状态栏
  DrawHudStatus(painter, transform);
}

/**
 * @brief draw hud status
 * @param painter, transform
 * @note transform may be post by reference
 */
void HUDView::DrawHudStatus(QPainter &painter, QTransform transform) {
  painter.save();

  // 绘制状态面板
  painter.setPen(Qt::white);
  painter.setBrush(QColor(0, 0, 0, 100));
  QPainterPath status_path;
  status_path.moveTo(-120, 90);
  status_path.lineTo(-120, 145);
  status_path.lineTo(120, 145);
  status_path.lineTo(120, 90);
  status_path.lineTo(90, 90);
  status_path.lineTo(80, 100);
  status_path.lineTo(-80, 100);
  status_path.lineTo(-90, 90);
  status_path.lineTo(-120, 90);
  painter.drawPath(status_path);

  // 绘制电池
  painter.setPen(Qt::yellow);
  painter.setBrush(Qt::transparent);
  QPainterPath battery_path;
  battery_path.moveTo(-91, 92);
  battery_path.lineTo(-91, 100);
  battery_path.lineTo(-111, 100);
  battery_path.lineTo(-111, 98);
  battery_path.lineTo(-113, 98);
  battery_path.lineTo(-113, 94);
  battery_path.lineTo(-111, 94);
  battery_path.lineTo(-111, 92);
  battery_path.lineTo(-91, 92);
  painter.drawPath(battery_path);
  painter.setPen(Qt::white);
  QFont font("楷体", 5);
  painter.setFont(font);
  painter.drawText(-115, 107, QObject::tr("battery"));
  // 绘制电量
  painter.setPen(Qt::yellow);
  painter.setBrush(Qt::yellow);
  int remain = (1 - battery_remain_ / 100) * 20;
  painter.drawRect(-111 + remain, 92, 20 - remain, 8);


  // 绘制 GPS
  painter.setPen(Qt::yellow);
  if (gps_status_) {
    painter.setBrush(Qt::yellow);
  } else {
    painter.setBrush(Qt::transparent);
  }
  painter.drawEllipse(98, 92, 8, 8);
  painter.setPen(Qt::white);
  painter.drawText(96, 107, QObject::tr("GPS"));

  painter.setPen(Qt::yellow);
  font.setPointSize(10);
  painter.setFont(font);
  painter.drawText(-30, 125, QObject::tr("NCHU APM"));


  painter.restore();
}


/**
 * @brief draw hud altitude [0, 100]
 * @param painter, transform
 * @note transform may be post by reference
 *       unit need modify
 */
void HUDView::DrawHudAltitude(QPainter &painter, QTransform transform) {
  painter.save();

  // 绘制高度竖直矩形
  painter.setPen(Qt::white);
  painter.setBrush(QColor(255, 255, 255, 25));
  painter.drawRect(130, -100, 50, 200);

  // 绘制高度单位
  painter.drawRect(130, 100, 50, 20);
  QFont font("楷体", 15);
  painter.setFont(font);
  painter.drawText(130 + 20, 100 + 15, QObject::tr("m"));

  // 绘制高度刻度盘 etc:[0, 100]
  painter.setPen(Qt::white);
  font.setPointSize(10);
  painter.setFont(font);
  int y_offset = 100;
  int long_line = 8;
  int short_line = 5;
  for (int s = altitude_ - 20; s < 101; s++) {
    if (s % 10 == 0) {
      painter.drawLine(130, 0 + y_offset, 130 + long_line, 0 + y_offset);
      painter.drawText(130 + long_line + 5, 0 + y_offset + 5, QString::number(s));
    } else {
      painter.drawLine(130, 0 + y_offset, 130 + short_line, 0 + y_offset);
    }

    if (y_offset < -90) {
      painter.setPen(Qt::transparent);
    }

    y_offset -= 5;
  }

  // 绘制高度显示标志
  painter.setPen(Qt::black);
  painter.setBrush(Qt::black);
  QPainterPath speed_show_path;
  speed_show_path.moveTo(178 + 1, -7);
  speed_show_path.lineTo(178 + 1, 7);
  speed_show_path.lineTo(178 + 1 - 35, 7);
  speed_show_path.lineTo(178 + 1 - 40, 0);
  speed_show_path.lineTo(178 + 1 - 35, -7);
  speed_show_path.lineTo(178 + 1, -7);
  painter.drawPath(speed_show_path);

  // 绘制高度
  painter.setPen(Qt::white);
  font.setPointSize(10);
  painter.setFont(font);
  painter.drawText(180 - 25, 5, QString::number(altitude_));
  painter.restore();
}

/**
 * @brief draw hud speed[0, 100]
 * @param painter, transform
 * @note transform may be post by reference
 *       unit need modify
 */
void HUDView::DrawHudSpeed(QPainter &painter, QTransform transform) {
  painter.save();

  // 绘制速度竖直矩形
  painter.setPen(Qt::white);
  painter.setBrush(QColor(255, 255, 255, 25));
  painter.drawRect(-180, -100, 50, 200);

  // 绘制速度单位
  painter.drawRect(-180, 100, 50, 20);
  QFont font("楷体", 15);
  painter.setFont(font);
  painter.drawText(-180 + 10, 100 + 15, QObject::tr("m/s"));

  // 绘制速度刻度盘 etc:[0, 100]
  painter.setPen(Qt::white);
  font.setPointSize(10);
  painter.setFont(font);
  int y_offset = 100;
  int long_line = 8;
  int short_line = 5;
  for (int s = speed_ - 20; s < 101; s++) {
    if (s % 10 == 0) {
      painter.drawLine(-130, 0 + y_offset, -130 - long_line, 0 + y_offset);
      if (s >= 0) {
        painter.drawText(-130 - long_line - 20, 0 + y_offset + 5, QString::number(s));
      }
    } else {
      painter.drawLine(-130, 0 + y_offset, -130 - short_line, 0 + y_offset);
    }

    if (y_offset < -90) {
      painter.setPen(Qt::transparent);
    }
    y_offset -= 5;
  }

  // 绘制速度显示标志
  painter.setPen(Qt::black);
  painter.setBrush(Qt::black);
  QPainterPath speed_show_path;
  speed_show_path.moveTo(-178 - 1, -7);
  speed_show_path.lineTo(-178 - 1, 7);
  speed_show_path.lineTo(-178 - 1 + 35, 7);
  speed_show_path.lineTo(-178 - 1 + 40, 0);
  speed_show_path.lineTo(-178 - 1 + 35, -7);
  speed_show_path.lineTo(-178 - 1, -7);
  painter.drawPath(speed_show_path);

  // 绘制速度
  painter.setPen(Qt::white);
  font.setPointSize(10);
  painter.setFont(font);

  // 50 换成速度变量
  painter.drawText(-178 + 12, 5, QString::number(speed_));

  painter.restore();
}



/**
 * @brief draw hud pitch scale
 * @param painter, transform
 * @note transform may be post by reference
 */
void HUDView::DrawHudYawScale(QPainter &painter, QTransform transform) {
  painter.save();

  // 绘制透明矩形
  painter.setPen(Qt::white);
  painter.setBrush(Qt::transparent);
  painter.drawRect(-90, -150, 180, 16);

  // 在矩形中绘制刻度，每份 10 度
  int interval = 0;
  for (int i = 0; i < 360 / 10; i++) {
    painter.drawLine(-85 + interval, -150, -85 + interval, -145);
    interval += 5;
  }

  // 绘制偏航游标
  double convert_factor = 0.5;
  transform.translate(yaw_angle_ * convert_factor, 0);
  painter.setWorldTransform(transform);
  painter.setBrush(Qt::red);
  const int triangle[3][2]={ {0, -140}, {-4, -135}, {4, -135} };
  painter.drawPolygon(QPolygon(3, &triangle[0][0]));

  // 绘制偏航值托板
  painter.drawLine(4, -135, 12 + 10, -135);

  // 绘制偏航值
  QFont font("楷体", 7);
  painter.setFont(font);
  painter.drawText(5, -137, QString::number(yaw_angle_));
  painter.restore();
}

/**
 * @brief draw hud pitch scale
 * @param painter, transform
 * @note transform may be post by reference
 */
void HUDView::DrawHudPitchScale(QPainter &painter, QTransform transform) {
  painter.save();

  QPen pen(Qt::white, 2);
  painter.setPen(pen);
  // 以俯仰角度为循环单位，俯仰角一更新就重绘刻度线
  int angle = 0;
  int y_offset = 210;
  for (int pitch_angle = -150 + angle + 80; pitch_angle < 90; pitch_angle += 10) {
    painter.drawLine(-30, y_offset, 30, y_offset);
    if (y_offset + 15 < 50) {
      painter.drawLine(-30 + 10, y_offset + 15, 30 - 10, y_offset + 15);
    }
    painter.drawText(-30 - 17, y_offset, QString::number(pitch_angle));

    if (y_offset > 90 || y_offset < -50) {
      painter.setPen(Qt::transparent);
    } else {
      painter.setPen(Qt::white);
    }
    y_offset -= 30;
  }


  painter.restore();
#if 0
  painter.setPen(Qt::white);

  int pitch = 10;
  // 设置俯仰角度
  transform.translate(0, pitch_angle_);
  painter.setWorldTransform(transform);


  //[-90, 90]
  int y_offset = -80;

  for (int angle = pitch_angle_ - 90 + 70; angle < 95; angle += 5) {
    painter.drawLine(-40, 0 + y_offset, 40, 0 + y_offset);
    painter.drawText(-40 - 17, 0 + y_offset, QString::number(std::abs(angle)));
    painter.drawText(40 + 5, 0 + y_offset, QString::number(std::abs(angle)));

    painter.drawLine(-20, 0 + y_offset + 10, 20, 0 + y_offset + 10);
    //painter.drawText(-20 - 20, 0 + y_offset, QString::number(angle));
    //painter.drawText(20, 0 + y_offset, QString::number(angle));

    if (y_offset > 50) {
      //painter.setPen(Qt::transparent);
    }



    y_offset += 20;
  }

  painter.restore();
#endif
#if 0
  painter.save();
  painter.setPen(Qt::white);

  // 设置俯仰角度
  transform.translate(0, pitch_angle_);
  painter.setWorldTransform(transform);

  //       20   10    0    10   20

  /*
  int k = -80;
  // 循环俯仰刻度
  for (int i = static_cast<int>(pitch_angle_) - 20; i <  pitch_angle_ + 25; i += 5) {
    if (i % 10 == 0) {
      painter.drawLine(-40, 0 + k, 40, 0 + k);
      painter.drawText(-40 - 20, 0 + k, QString::number(i));
      painter.drawText(40, 0 + k, QString::number(i));
    } else {
      painter.drawLine(-20, 0 + k, 20, 0 + k);
      painter.drawText(-20 - 20, 0 + k, QString::number(i));
      painter.drawText(20, 0 + k, QString::number(i));
    }

    k += 20;
  }
  */

  int k = -80;
  int long_line = 25;
  int short_line = 10;
  // 循环俯仰刻度, need to be modify
  for (int i = 20; i > -25; i -= 5) {
    if (i % 10 == 0) {
      painter.drawLine(-long_line, 0 + k, long_line, 0 + k);
      if (i == 0) {
        painter.drawText(-long_line - 10, 0 + k + 4, QString("0"));
        painter.drawText(long_line + 4, 0 + k + 4, QString("0"));
      } else {
        painter.drawText(-long_line - 15, 0 + k + 4, QString::number(std::abs(i)));
        painter.drawText(long_line + 4, 0 + k + 4, QString::number(std::abs(i)));
      }

    } else {
      painter.drawLine(-short_line, 0 + k, short_line, 0 + k);
      // 暂时显示短刻度值
      /*
      if (std::abs(i) < 10) {
        painter.drawText(-short_line - 15, 0 + k + 4, QString("0") + QString::number(std::abs(i)));
        painter.drawText(short_line + 4, 0 + k + 4, QString("0") + QString::number(std::abs(i)));
      } else {
        painter.drawText(-short_line - 15, 0 + k + 4, QString::number(std::abs(i)));
        painter.drawText(short_line + 4, 0 + k + 4, QString::number(std::abs(i)));
      }
      */

    }

    k += 20;
  }

  painter.restore();

#endif
}


/**
 * @brief draw hud Crossbow
 * @param painter, transform
 * @note transform may be post by reference
 */
void HUDView::DrawHudCrossbow(QPainter &painter, QTransform transform) {
  painter.save();

  // 横滚移动
  transform.rotate(roll_angle_);
  // 俯仰移动, y 轴向上为负，因此需要取相反数
  transform.translate(0, -pitch_angle_);
  painter.setWorldTransform(transform);

  QPen pen(QColor(255, 0, 255), 2);
  // 绘制十字架
  painter.setPen(pen);
  // 水平
  painter.drawLine(-50, 0, 50, 0);
  // 竖直
  painter.drawLine(0, -50, 0, 50);

  // 左边黑色水平标志
  pen.setColor(Qt::white);
  painter.setPen(pen);
  painter.setBrush(Qt::black);
  QPainterPath path;
  path.moveTo(-50, -2);
  path.lineTo(-50, 10);
  path.lineTo(-55, 10);
  path.lineTo(-55, 2);
  path.lineTo(-85, 2);
  path.lineTo(-85, -2);
  path.lineTo(-50, -2);

  painter.drawPath(path);

  // 右边黑色水平标记
  path.moveTo(50, -2);
  path.lineTo(50, 10);
  path.lineTo(55, 10);
  path.lineTo(55, 2);
  path.lineTo(85, 2);
  path.lineTo(85, -2);
  path.lineTo(50, -2);
  painter.drawPath(path);

  // 绘制圆
  painter.setPen(Qt::white);
  painter.setBrush(Qt::transparent);
  painter.drawEllipse(-10, -10, 20, 20);

  // 绘制中心黑点
  pen.setColor(Qt::black);
  pen.setWidth(3);
  painter.setPen(pen);
  painter.drawPoint(0, 0);

  painter.restore();
}

/**
 * @brief draw hud radian
 * @param painter, transform
 * @note transform may be post by reference
 *        roll scale need to modify!
 */
void HUDView::DrawHudRollRadian(QPainter &painter, QTransform transform) {
  painter.save();

  painter.setPen(Qt::white);
  QRect arc_rect(-96, -110, 192, 160);
  int start_angle = 20 * 16;
  int span_angle = 140 * 16;
  // 绘制 160 度的圆弧
  painter.drawArc(arc_rect, start_angle, span_angle);

  painter.save();
  // 绘制圆弧刻度 [-60, 60]
  int long_scale_len = 8;
  int short_scale_len = 5;
  painter.rotate(-60);
  // no good
  for (int i = -60; i < 70; i += 10) {
    if (i == 60) {
      painter.drawLine(0 - 4, -110 + 3, 0 - 4, -(110 + long_scale_len) + 3);
      painter.drawText(0 - 4 - 5, -110 + 3 - 10, QString::number(60));
    } else if (i == -60) {
      painter.drawLine(0 + 4, -110 + 3, 0 + 4, -(110 + long_scale_len) + 3);
      painter.drawText(0 - 4 - 5, -110 + 3 - 10, QString::number(-60));
    } else if (i == 30){
      painter.drawLine(0, -110, 0, -(110 + long_scale_len));
      painter.drawText(0 - 4 - 2, -110 + 3 - 12, QString::number(30));
    } else if (i == -30){
      painter.drawLine(0, -110, 0, -(110 + long_scale_len));
      painter.drawText(0 - 4 - 5 - 3, -110 + 3 - 10 - 2, QString::number(-30));
    } else if (i == 0){
      painter.drawLine(0, -110, 0, -(110 + long_scale_len));
      painter.drawText(0 - 4 + 1, -110 + 3 - 10 - 3, QString::number(0));
    } else {
      painter.drawLine(0, -110, 0, -(110 + short_scale_len));
    }
    painter.rotate(10);
  }

  painter.restore();

  // 绘制横滚刻度标记
  transform.rotate(roll_angle_);
  painter.setWorldTransform(transform);
  painter.setBrush(Qt::white);
  const int triangle[3][2]={ {0, -109}, {-4, -103}, {4, -103} };
  painter.drawPolygon(QPolygon(3, &triangle[0][0]));

  painter.restore();
}


/**
 * @brief draw hud bg
 * @param painter, transform
 * @note transform may be post by reference
 *       pitch_angle_ need to be calc ! Don't forget !
 */
void HUDView::DrawHudBgColor(QPainter &painter, QTransform transform) {
  painter.save();

  // 移动坐标原点的 y 坐标可以控制俯仰角度，需要计算
  transform.translate(0, -pitch_angle_);

  // 旋转画布可以控制横滚角度
  transform.rotate(roll_angle_);
  painter.setWorldTransform(transform);

  painter.setPen(Qt::NoPen);
  // 绘制天空
  painter.setBrush(QColor(65, 105, 225));
  painter.drawRect(-width(), -height(), width() * 2, height());
  // 绘制地面
  painter.setBrush(QColor(160, 82, 45));
  painter.drawRect(-width(), 0, width() * 2, height());
  // 绘制箭头标记
  painter.setBrush(Qt::yellow);
  painter.setPen(QColor(255, 165, 0));
  QPainterPath path;
  path.moveTo(0, 10);
  path.lineTo(-40, 40);
  path.lineTo(-20, 40);
  path.lineTo(0, 10);
  path.lineTo(40, 40);
  path.lineTo(20, 40);
  path.lineTo(0, 10);
  painter.drawPath(path);

  // 绘制左右两个黄色矩形标记, 长 30，宽 5
  painter.drawRect(-120, 10, 30, 5);
  painter.drawRect(90, 10, 30, 5);

  painter.restore();
}

void HUDView::setRoll(const double roll_angle) {
  roll_angle_ = roll_angle;
}

void HUDView::setPitch(const double pitch_angle) {
  pitch_angle_ = pitch_angle;
}

void HUDView::setYaw(const double yaw_angle) {
  yaw_angle_ = yaw_angle;
}

void HUDView::setSpeed(const double speed) {
  speed_ = speed;
}

void HUDView::setAltitude(const double altitude) {
  altitude_ = altitude;
}

void HUDView::UpdateView() {
  this->update();
}





