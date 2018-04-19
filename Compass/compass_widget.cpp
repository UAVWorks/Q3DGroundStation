#include "compass_widget.h"
#include "ui_compasswidget.h"

#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QTimer>


CompassWidget::CompassWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CompassWidget),
  bg_need_redraw_(true)
{
  ui->setupUi(this);
  // 不用按下鼠标按键也能触发鼠标移动
  setMouseTracking(true);

  angle_ = 0.0;
}

CompassWidget::~CompassWidget()
{
  delete ui;
}


/**
 * @brief draw compass static bg
 *
 * @note need to be draw only resize
 */
void CompassWidget::DrawStaticBarkGround(QPainter &painter) {
  QPen pen(Qt::green, 1);
  painter.setPen(pen);

  // 静态背景需要在改变大小的时候绘制一次

  int r = 120;

  // 绘图坐标不变，坐标系旋转
  painter.drawEllipse(-r, -r, 2 * r, 2 * r);
  //painter.drawLine(0, 120, 0, 130);

  int start_y = -r;
  int start_x = 0;
  int scale_short_len = 8;
  int scale_long_len = 14;

  // 绘制：外圆 + 长刻度 + 短刻度 + 长刻度值
  for (int angle = 0; angle < 360; angle += 15) {
    if (angle % 45 == 0) {
      // 长指针
      painter.drawLine(start_x, start_y, start_x, start_y - scale_long_len);
      // 绘制刻度值
      if (angle == 0) {
        // 画矩形框
        painter.save();
        painter.setPen(Qt::white);
        painter.drawRect(start_x - 12, start_y - 30, 24, 12);
        painter.restore();
        // 画 000
        painter.drawText(start_x - 9, start_y - 20, QObject::tr("000"));
      } else if (angle < 100) {
        painter.drawText(start_x - 9, start_y - 20, QObject::tr("0") + QString::number(angle));
      } else {
        painter.drawText(start_x - 9, start_y - 20, QString::number(angle));
      }
    } else {
      // 短指针
      painter.drawLine(start_x, start_y, start_x, start_y - scale_short_len);
    }
    painter.rotate(15);
  }

  // 开始绘制白色内圈
  pen.setWidth(1);
  pen.setColor(Qt::white);
  painter.setPen(pen);

  // 白色内圈半径 115
  int write_r = 115;
  painter.drawEllipse(-write_r, -write_r, 2 * write_r, 2 * write_r);

  int white_scale_long_len = 10;
  int white_scale_short_len = 5;
  int white_start_x = 0;
  int white_start_y = -write_r;

  // 内圈刻度值字体较外圈小
  QFont font("楷体", 7);
  painter.setFont(font);
  for (double angle = 0; angle < 360; angle += 7.5) {
    if (static_cast<int>(angle) % 30 == 0) {
      // 绘制长刻度
      painter.drawLine(white_start_x, white_start_y, white_start_x, white_start_y + white_scale_long_len);
      if (angle == 0.0) {
        painter.drawText(white_start_x - 8, white_start_y + 20, QObject::tr("000"));
      } else if (angle < 100.0) {
        // 小于 100 的两位数前面加个 0
        painter.drawText(white_start_x - 8, white_start_y + 20, QObject::tr("0") + QString::number(angle));
      } else {
        painter.drawText(white_start_x - 8, white_start_y + 20, QString::number(angle));
      }
    } else {
      //绘制短刻度
      painter.drawLine(white_start_x, white_start_y, white_start_x, white_start_y + white_scale_short_len);
    }
    painter.rotate(7.5);
  }
}

void CompassWidget::resizeEvent(QResizeEvent *event) {
  // 界面大小改变，重绘静态背景
  bg_need_redraw_ = true;
}

void CompassWidget::paintEvent(QPaintEvent *event) {
  // 坐标转换对象
  QTransform transform;
  // 移动坐标原点位 widget 中心
  transform.translate(width() / 2, height() / 2);
  // 设置控件缩放，当窗口改变大小时，绘制的内容会变化大小
  int side = qMin(width(), height());
  transform.scale(side / 300.0, side / 300.0);

  QPainter painter;
  // 控件改变大小时重绘静态背景
  if (bg_need_redraw_) {
    // 在 temp_pix_ 位图上绘制
    temp_pix_ = QPixmap(width(), height());
    painter.begin(&temp_pix_);

    // 设置黑色背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(rect());
    // 开启抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    // 设置坐标转换节点
    painter.setWorldTransform(transform);
    // 画静态背景
    DrawStaticBarkGround(painter);

    // 结束在 temp_pix_ 位图上绘制
    painter.end();

    // 取消重绘标记
    bg_need_redraw_ = false;
  }

  // 将 temp_pix_ 拷贝到 this 画布上
  painter.begin(this);
  painter.drawPixmap(0, 0, temp_pix_);

  // 在新的 this 画布上绘图，必须重新初始化，否则看不到图像
  painter.setPen(Qt::white);
  painter.setWorldTransform(transform);

  // 保存状态
  painter.save();

  // 旋转指南针坐标系 angle_ 度
  transform.rotate(angle_);
  painter.setWorldTransform(transform);

  QBrush brush(Qt::green);
  painter.setBrush(brush);
  // 绘制中间指南针路径
  QPainterPath compass_path;
  compass_path.moveTo(0, -25);
  compass_path.lineTo(20, 10);
  compass_path.lineTo(0, 0);
  compass_path.lineTo(-20, 10);
  compass_path.lineTo(0, -25);
  painter.drawPath(compass_path);

  // 恢复状态，取消坐标系旋转，防止角度值旋转
  painter.restore();

  // 绘制当前角度值
  QFont font2(QObject::tr("楷体"), 11);
  painter.setFont(font2);
  painter.drawRect(-30, -70, 60, 20);
  if (angle_ == 0.0) {
    painter.drawText(-11, -55, QString("000"));
  } else if (angle_ < 100.0) {
    painter.drawText(-11, -55, QString("0") + QString::number(angle_));
  } else {
    painter.drawText(-11, -55, QString::number(angle_));
  }

  painter.end();
}


void CompassWidget::setAngle(double angle) {
  angle_ = angle;
}

