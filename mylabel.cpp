#include "mylabel.h"
#include <QPen>
#include<QPainter>
#include <QColor>

MyLabel::MyLabel()
{

}
MyLabel::MyLabel (QWidget *parent){
    this->setParent(parent);
    this->sp = QPoint(px, py);
    this->width = w;
    this->height = h;
    this->linesize = 1;
    this->color = QColor(Qt::red);
}

void MyLabel::paintEvent(QPaintEvent * event){
    QLabel::paintEvent(event);
    QPainter painter(this);
    QPen pen;
    pen.setColor(this->color);
    pen.setWidth(this->linesize);
    painter.setPen(pen);

    painter.drawRect(sp.x(), sp.y(), width, height);
}
