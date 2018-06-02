#include "tachometer.h"
#include "ui_tachometer.h"

#include <QPainter>
#include <QTimer>

Tachometer::Tachometer(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Tachometer)
{
  ui->setupUi(this);

  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &Tachometer::TimerUpdate);
  //timer_->start(100);
  show_text_ = QString("AirSpeed");
  speed_ = 0;
  scale_ = 1000;
}

Tachometer::~Tachometer()
{
  delete ui;
}


void Tachometer::TimerUpdate() {
  speed_ += 100;
  if (speed_ > 10000) {
    speed_ = 0;
  }
  this->update();
}

void Tachometer::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::black);
  painter.drawRect(rect());

  // ����ת������
  QTransform transform;
  // ��������ԭ��Ϊ�ؼ�����
  transform.translate(width() / 2, height() / 2);
  // ���ÿؼ����ţ������ڸı��Сʱ�����Ƶ����ݻ�仯��С
  int side = qMin(width(), height());
  transform.scale(side / 300.0, side / 300.0);
  painter.setWorldTransform(transform);
  // ���������
  painter.setRenderHint(QPainter::Antialiasing);

  // ���ƾ�̬����
  DrawStaticBarkGround(painter);

  // ���ƶ�̬ת��ָ���ת��ֵ
  DrawDynamicPointer(painter, transform);

  painter.end();
}

void Tachometer::DrawStaticBarkGround(QPainter &painter) {
  painter.save();

  // ���Ʊ���Բ
  QPen pen(QColor(0, 0, 10));
  painter.setPen(pen);
  QBrush brush(QColor(0, 0, 10));
  painter.setBrush(brush);
  painter.drawEllipse(-145, -145, 290, 290);

  // ���ƿ̶�
  painter.save();
  pen.setColor(Qt::white);
  pen.setWidth(2);
  painter.setPen(pen);
  QFont font("����", 18);
  painter.setFont(font);
  painter.rotate(-125);
  int long_line = 15;
  int short_line = 10;

  for (int i = 0; i < 51; i++) {
    if (i < 15) {
      pen.setColor(Qt::white);
    } else if (i < 35) {
      pen.setColor(Qt::yellow);
    } else {
      pen.setColor(Qt::red);
    }
    painter.setPen(pen);

    if (i % 5 == 0) {
      painter.drawLine(0, -100, 0, -(100 + long_line));

      // ���ƿ̶�ֵ
      painter.drawText(0 - 13, -125, QString::number(i * 2));

    } else {
      painter.drawLine(0, -100, 0, -(100 + short_line));
    }
    painter.rotate(5);
  }
  painter.restore();

  // ���ƾ���
  pen.setColor(Qt::yellow);
  painter.setPen(pen);
  painter.setBrush(Qt::transparent);
  painter.drawRect(-25, 75, 50, 25);

  pen.setColor(Qt::white);
  pen.setWidth(2);
  painter.setPen(pen);
  font.setPointSize(20);
  painter.setFont(font);
  //painter.drawText(-25, 65, QString("x") + QString::number(scale_));

  // draw speed text
  painter.drawText(-50, 65, show_text_);

  painter.restore();
}

void Tachometer::DrawDynamicPointer(QPainter &painter, QTransform transform) {
  painter.save();

  // ����ʵʱת��ֵ
  QPen pen(Qt::white, 2);
  QFont font("����", 15);
  painter.setFont(font);

  if (speed_ / scale_ < 3) {
    pen.setColor(Qt::white);
  } else if (speed_ / scale_ < 7) {
    pen.setColor(Qt::yellow);
  } else {
    pen.setColor(Qt::red);
  }

  painter.setPen(pen);
  painter.drawText(-20, 95, QString::number(speed_));

  transform.rotate((speed_ / scale_) * 25);
  painter.setWorldTransform(transform);
  // ����ָ��ͷ��

  QPainterPath pointer_path;
  pointer_path.moveTo(-10, 0);
  pointer_path.lineTo(-80, 56);
  pointer_path.lineTo(-4, 9);

  pen.setColor(Qt::white);
  pen.setWidth(2);
  painter.setPen(pen);
  QBrush brush(Qt::red);
  painter.setBrush(brush);
  painter.drawPath(pointer_path);

  // ��������Բ
  pen.setColor(Qt::white);
  pen.setWidth(3);
  brush.setColor(Qt::black);
  painter.setBrush(brush);
  painter.setPen(pen);
  painter.drawEllipse(-10, -10, 20, 20);

  // ����ָ��β��
  pen.setColor(Qt::white);
  pen.setWidth(2);
  painter.setPen(pen);
  brush.setColor(Qt::white);
  painter.setBrush(Qt::white);
  QPainterPath pointer_path2;
  pointer_path2.moveTo(5, -8);
  pointer_path2.lineTo(20, -25);
  pointer_path2.lineTo(31, -10);
  pointer_path2.lineTo(12, -2);
  painter.drawPath(pointer_path2);

  painter.restore();
}

void Tachometer::setSpeed(double speed) {
  speed_ = speed;
}


void Tachometer::setScale(int scale) {
  scale_ = scale;
}

void Tachometer::setShowText(QString show_text) {
  show_text_ = show_text;
}

void Tachometer::UpdateView() {
    this->update();
}
