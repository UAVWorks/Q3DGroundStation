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
  // ���ð�����갴��Ҳ�ܴ�������ƶ�
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

  // ��̬������Ҫ�ڸı��С��ʱ�����һ��

  int r = 120;

  // ��ͼ���겻�䣬����ϵ��ת
  painter.drawEllipse(-r, -r, 2 * r, 2 * r);
  //painter.drawLine(0, 120, 0, 130);

  int start_y = -r;
  int start_x = 0;
  int scale_short_len = 8;
  int scale_long_len = 14;

  // ���ƣ���Բ + ���̶� + �̶̿� + ���̶�ֵ
  for (int angle = 0; angle < 360; angle += 15) {
    if (angle % 45 == 0) {
      // ��ָ��
      painter.drawLine(start_x, start_y, start_x, start_y - scale_long_len);
      // ���ƿ̶�ֵ
      if (angle == 0) {
        // �����ο�
        painter.save();
        painter.setPen(Qt::white);
        painter.drawRect(start_x - 12, start_y - 30, 24, 12);
        painter.restore();
        // �� 000
        painter.drawText(start_x - 9, start_y - 20, QObject::tr("000"));
      } else if (angle < 100) {
        painter.drawText(start_x - 9, start_y - 20, QObject::tr("0") + QString::number(angle));
      } else {
        painter.drawText(start_x - 9, start_y - 20, QString::number(angle));
      }
    } else {
      // ��ָ��
      painter.drawLine(start_x, start_y, start_x, start_y - scale_short_len);
    }
    painter.rotate(15);
  }

  // ��ʼ���ư�ɫ��Ȧ
  pen.setWidth(1);
  pen.setColor(Qt::white);
  painter.setPen(pen);

  // ��ɫ��Ȧ�뾶 115
  int write_r = 115;
  painter.drawEllipse(-write_r, -write_r, 2 * write_r, 2 * write_r);

  int white_scale_long_len = 10;
  int white_scale_short_len = 5;
  int white_start_x = 0;
  int white_start_y = -write_r;

  // ��Ȧ�̶�ֵ�������ȦС
  QFont font("����", 7);
  painter.setFont(font);
  for (double angle = 0; angle < 360; angle += 7.5) {
    if (static_cast<int>(angle) % 30 == 0) {
      // ���Ƴ��̶�
      painter.drawLine(white_start_x, white_start_y, white_start_x, white_start_y + white_scale_long_len);
      if (angle == 0.0) {
        painter.drawText(white_start_x - 8, white_start_y + 20, QObject::tr("000"));
      } else if (angle < 100.0) {
        // С�� 100 ����λ��ǰ��Ӹ� 0
        painter.drawText(white_start_x - 8, white_start_y + 20, QObject::tr("0") + QString::number(angle));
      } else {
        painter.drawText(white_start_x - 8, white_start_y + 20, QString::number(angle));
      }
    } else {
      //���ƶ̶̿�
      painter.drawLine(white_start_x, white_start_y, white_start_x, white_start_y + white_scale_short_len);
    }
    painter.rotate(7.5);
  }
}

void CompassWidget::resizeEvent(QResizeEvent *event) {
  // �����С�ı䣬�ػ澲̬����
  bg_need_redraw_ = true;
}

void CompassWidget::paintEvent(QPaintEvent *event) {
  // ����ת������
  QTransform transform;
  // �ƶ�����ԭ��λ widget ����
  transform.translate(width() / 2, height() / 2);
  // ���ÿؼ����ţ������ڸı��Сʱ�����Ƶ����ݻ�仯��С
  int side = qMin(width(), height());
  transform.scale(side / 300.0, side / 300.0);

  QPainter painter;
  // �ؼ��ı��Сʱ�ػ澲̬����
  if (bg_need_redraw_) {
    // �� temp_pix_ λͼ�ϻ���
    temp_pix_ = QPixmap(width(), height());
    painter.begin(&temp_pix_);

    // ���ú�ɫ����
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(rect());
    // ���������
    painter.setRenderHint(QPainter::Antialiasing);
    // ��������ת���ڵ�
    painter.setWorldTransform(transform);
    // ����̬����
    DrawStaticBarkGround(painter);

    // ������ temp_pix_ λͼ�ϻ���
    painter.end();

    // ȡ���ػ���
    bg_need_redraw_ = false;
  }

  // �� temp_pix_ ������ this ������
  painter.begin(this);
  painter.drawPixmap(0, 0, temp_pix_);

  // ���µ� this �����ϻ�ͼ���������³�ʼ�������򿴲���ͼ��
  painter.setPen(Qt::white);
  painter.setWorldTransform(transform);

  // ����״̬
  painter.save();

  // ��תָ��������ϵ angle_ ��
  transform.rotate(angle_);
  painter.setWorldTransform(transform);

  QBrush brush(Qt::green);
  painter.setBrush(brush);
  // �����м�ָ����·��
  QPainterPath compass_path;
  compass_path.moveTo(0, -25);
  compass_path.lineTo(20, 10);
  compass_path.lineTo(0, 0);
  compass_path.lineTo(-20, 10);
  compass_path.lineTo(0, -25);
  painter.drawPath(compass_path);

  // �ָ�״̬��ȡ������ϵ��ת����ֹ�Ƕ�ֵ��ת
  painter.restore();

  // ���Ƶ�ǰ�Ƕ�ֵ
  QFont font2(QObject::tr("����"), 11);
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

