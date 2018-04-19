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

  // ����ת������
  QTransform transform;
  // ��������ԭ��Ϊ�ؼ�����
  transform.translate(width() / 2, height() / 2);
  // ���ÿؼ����ţ������ڸı��Сʱ�����Ƶ����ݻ�仯��С
  int side = qMin(width(), height());
  transform.scale(side / 300.0, side / 300.0);
  painter.setWorldTransform(transform);

  painter.setRenderHint(QPainter::Antialiasing);

  // ������-�Ʊ���ɫ
  DrawHudBgColor(painter, transform);
  // ���ƺ������
  DrawHudRollRadian(painter, transform);
  // ���Ƹ����̶�
  DrawHudPitchScale(painter, transform);
  // ����ʮ�ָ�
  DrawHudCrossbow(painter, transform);
  // ����ƫ���̶�
  DrawHudYawScale(painter, transform);
  // �����ٶ�
  DrawHudSpeed(painter, transform);
  // ���Ƹ߶�
  DrawHudAltitude(painter, transform);
  // ����״̬��
  DrawHudStatus(painter, transform);
}

/**
 * @brief draw hud status
 * @param painter, transform
 * @note transform may be post by reference
 */
void HUDView::DrawHudStatus(QPainter &painter, QTransform transform) {
  painter.save();

  // ����״̬���
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

  // ���Ƶ��
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
  QFont font("����", 5);
  painter.setFont(font);
  painter.drawText(-115, 107, QObject::tr("battery"));
  // ���Ƶ���
  painter.setPen(Qt::yellow);
  painter.setBrush(Qt::yellow);
  int remain = (1 - battery_remain_ / 100) * 20;
  painter.drawRect(-111 + remain, 92, 20 - remain, 8);


  // ���� GPS
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

  // ���Ƹ߶���ֱ����
  painter.setPen(Qt::white);
  painter.setBrush(QColor(255, 255, 255, 25));
  painter.drawRect(130, -100, 50, 200);

  // ���Ƹ߶ȵ�λ
  painter.drawRect(130, 100, 50, 20);
  QFont font("����", 15);
  painter.setFont(font);
  painter.drawText(130 + 20, 100 + 15, QObject::tr("m"));

  // ���Ƹ߶ȿ̶��� etc:[0, 100]
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

  // ���Ƹ߶���ʾ��־
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

  // ���Ƹ߶�
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

  // �����ٶ���ֱ����
  painter.setPen(Qt::white);
  painter.setBrush(QColor(255, 255, 255, 25));
  painter.drawRect(-180, -100, 50, 200);

  // �����ٶȵ�λ
  painter.drawRect(-180, 100, 50, 20);
  QFont font("����", 15);
  painter.setFont(font);
  painter.drawText(-180 + 10, 100 + 15, QObject::tr("m/s"));

  // �����ٶȿ̶��� etc:[0, 100]
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

  // �����ٶ���ʾ��־
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

  // �����ٶ�
  painter.setPen(Qt::white);
  font.setPointSize(10);
  painter.setFont(font);

  // 50 �����ٶȱ���
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

  // ����͸������
  painter.setPen(Qt::white);
  painter.setBrush(Qt::transparent);
  painter.drawRect(-90, -150, 180, 16);

  // �ھ����л��ƿ̶ȣ�ÿ�� 10 ��
  int interval = 0;
  for (int i = 0; i < 360 / 10; i++) {
    painter.drawLine(-85 + interval, -150, -85 + interval, -145);
    interval += 5;
  }

  // ����ƫ���α�
  double convert_factor = 0.5;
  transform.translate(yaw_angle_ * convert_factor, 0);
  painter.setWorldTransform(transform);
  painter.setBrush(Qt::red);
  const int triangle[3][2]={ {0, -140}, {-4, -135}, {4, -135} };
  painter.drawPolygon(QPolygon(3, &triangle[0][0]));

  // ����ƫ��ֵ�а�
  painter.drawLine(4, -135, 12 + 10, -135);

  // ����ƫ��ֵ
  QFont font("����", 7);
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
  // �Ը����Ƕ�Ϊѭ����λ��������һ���¾��ػ�̶���
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
  // ���ø����Ƕ�
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

  // ���ø����Ƕ�
  transform.translate(0, pitch_angle_);
  painter.setWorldTransform(transform);

  //       20   10    0    10   20

  /*
  int k = -80;
  // ѭ�������̶�
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
  // ѭ�������̶�, need to be modify
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
      // ��ʱ��ʾ�̶̿�ֵ
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

  // ����ƶ�
  transform.rotate(roll_angle_);
  // �����ƶ�, y ������Ϊ���������Ҫȡ�෴��
  transform.translate(0, -pitch_angle_);
  painter.setWorldTransform(transform);

  QPen pen(QColor(255, 0, 255), 2);
  // ����ʮ�ּ�
  painter.setPen(pen);
  // ˮƽ
  painter.drawLine(-50, 0, 50, 0);
  // ��ֱ
  painter.drawLine(0, -50, 0, 50);

  // ��ߺ�ɫˮƽ��־
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

  // �ұߺ�ɫˮƽ���
  path.moveTo(50, -2);
  path.lineTo(50, 10);
  path.lineTo(55, 10);
  path.lineTo(55, 2);
  path.lineTo(85, 2);
  path.lineTo(85, -2);
  path.lineTo(50, -2);
  painter.drawPath(path);

  // ����Բ
  painter.setPen(Qt::white);
  painter.setBrush(Qt::transparent);
  painter.drawEllipse(-10, -10, 20, 20);

  // �������ĺڵ�
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
  // ���� 160 �ȵ�Բ��
  painter.drawArc(arc_rect, start_angle, span_angle);

  painter.save();
  // ����Բ���̶� [-60, 60]
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

  // ���ƺ���̶ȱ��
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

  // �ƶ�����ԭ��� y ������Կ��Ƹ����Ƕȣ���Ҫ����
  transform.translate(0, -pitch_angle_);

  // ��ת�������Կ��ƺ���Ƕ�
  transform.rotate(roll_angle_);
  painter.setWorldTransform(transform);

  painter.setPen(Qt::NoPen);
  // �������
  painter.setBrush(QColor(65, 105, 225));
  painter.drawRect(-width(), -height(), width() * 2, height());
  // ���Ƶ���
  painter.setBrush(QColor(160, 82, 45));
  painter.drawRect(-width(), 0, width() * 2, height());
  // ���Ƽ�ͷ���
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

  // ��������������ɫ���α��, �� 30���� 5
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





