#ifndef MYLABEL_H
#define MYLABEL_H
#include <QLabel>
#include <QPoint>
#include <QColor>
#include <QPaintEvent>
#include <QImage>
#include <QRect>
#include <QPixmap>

const int px = 350;
const int py = 270;
const int w = 155;
const int h = 65;

class MyLabel : public QLabel
{
public:
    MyLabel();
    MyLabel (QWidget *parent);
    virtual void paintEvent(QPaintEvent * event);
private:
    QPoint sp;
    int width;
    int height;
    QColor color;
    int linesize;
};

#endif // MYLABEL_H
