#include "warn_item.h"
#include "ui_warnitem.h"

#include <QPainter>

WarnItem::WarnItem(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::WarnItem),
  brush_color_(Qt::transparent),
  text_(QObject::tr("WarnItem")),
  font_size_(10)
{
  ui->setupUi(this);
}

WarnItem::~WarnItem()
{
  delete ui;
}


void WarnItem::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  int s = 1;
  QRect rect(0 + s, 0 + s, width() - 2 * s, height() - 2 * s);
  // 开启抗锯齿
  painter.setRenderHint(QPainter::Antialiasing);

  QPen pen(Qt::white, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  painter.setPen(pen);

  // 最后一个 200 参数用于实现透明效果, 0 全透明，255 不透明
  QBrush brush(brush_color_, Qt::SolidPattern);
  painter.setBrush(brush);
  painter.drawRoundedRect(rect, 5, 5);

  QFont font("宋体", font_size_, QFont::Bold, false);
  font.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
  painter.setFont(font);
  pen.setColor(Qt::white);
  painter.setPen(pen);
  painter.drawText(rect, Qt::AlignCenter, text_);
}


void WarnItem::resizeEvent(QResizeEvent *event) {

}


void WarnItem::setText(QString text) {
  text_ = text;
}

void WarnItem::setWarnColor(QColor brush_color) {
  brush_color_ = brush_color;
}

void WarnItem::setFontSize(int font_size) {
  font_size_ = font_size;
}

void WarnItem::UpdateView() {
  this->update();
}









