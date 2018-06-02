#include "vsi.h"
#include "ui_vsi.h"

#include <QPainter>
#include <QTimer>

Vsi::Vsi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vsi)
{
    ui->setupUi(this);

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &Vsi::TimerUpdate);
    //timer_->start(100);
    //show_text_ = "";
    vis_ = 0;
}

Vsi::~Vsi()
{
    delete ui;
}

void Vsi::TimerUpdate() {
  vis_ += 1;
  if (vis_ > 10 ||vis_ < -10) {
    vis_ = 0;
  }
  this->update();
}

void Vsi::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

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
    // 开启抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);

  // 绘制静态背景
  DrawStaticBarkGround(painter);

  // 绘制动态转速指针和转速值
  DrawDynamicPointer(painter, transform);

  painter.end();
}

void Vsi::DrawStaticBarkGround(QPainter &painter) {
  painter.save();

  // 绘制背景圆
  QPen pen(QColor(0, 0, 10));
  painter.setPen(pen);
  QBrush brush(QColor(0, 0, 10));
  painter.setBrush(brush);
  painter.drawEllipse(-145, -145, 290, 290);

  // 绘制刻度
  painter.save();
  pen.setColor(Qt::white);
  pen.setWidth(2);
  painter.setPen(pen);
  QFont font("楷体", 18);
  painter.setFont(font);
  painter.rotate(-240);
  int long_line = 15;
  int mid_line = 10;
  int short_line = 5;

  for (int i = 0; i < 101; i++) {
    pen.setColor(Qt::white);
    painter.setPen(pen);

    if (i % 10 == 0) {
      pen.setWidth(3);
      painter.drawLine(0, -100, 0, -(100 + long_line));
      // 绘制刻度值
      painter.drawText(0 - 6, -120, QString::number(-10+i / 5));
    } else if(i % 5 == 0) {
      painter.drawLine(0, -100, 0, -(100 + mid_line));
    }else {
      painter.drawLine(0, -100, 0, -(100 + short_line));
    }
    painter.rotate(3);
  }
  painter.restore();



  pen.setColor(Qt::white);
  pen.setWidth(2);
  painter.setPen(pen);
  font.setPointSize(15);
  painter.setFont(font);
  painter.drawText(-15, 55,  QString("Vsi"));


  painter.restore();
}

void Vsi::DrawDynamicPointer(QPainter &painter, QTransform transform) {
  painter.save();

   //绘制实时转速值
  QPen pen(Qt::white, 2);

  transform.rotate(35);
  transform.rotate(vis_  * 15);
  painter.setWorldTransform(transform);
  // 绘制指针头部

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

  // 绘制中心圆
  pen.setColor(Qt::white);
  pen.setWidth(3);
  brush.setColor(Qt::black);
  painter.setBrush(brush);
  painter.setPen(pen);
  painter.drawEllipse(-10, -10, 20, 20);

  // 绘制指针尾部
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

void Vsi::setvis(double Vsi) {
  vis_ = Vsi;
}



void Vsi::UpdateView() {
    this->update();
}
