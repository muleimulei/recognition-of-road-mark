#ifndef WIDGET_H
#define WIDGET_H
#include <QString>
#include <QWidget>
#include "opencv/highgui.h"
#include <QTimer>
#include <QImage>
#include "ui_widget.h"
using namespace cv;

#include <QPushButton>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
private:
    QPushButton * captureButton;
    QString filepath;
    QImage MatToImage(const cv::Mat &mat);
    cv::VideoCapture capture;
    QTimer timer;
    cv::Mat top, left,right,l;
    cv::Mat currentframe; //当前帧

    IplImage *firstframe; //前一帧
    IplImage *tempframe;

    long long currentframeCount;
    long long totalframeCount;
    QImage currentPic;
    void addToPanel(QImage image);
    QWidget *picPanel;
    void showdiff();
    QImage IplImageToImage(IplImage *image);
    void calculategray(IplImage *tempframe);
    IplImage * getImageRect(IplImage *ori, const int px, const int py, const int w, const int h);

    void judgeType(IplImage &frame);
    void writemsg(QImage &frame);

    QImage addtolabel(QLabel *label, IplImage *im);// 将图像添加到显示器
    void addToall(IplImage &im);
    void removesmallregion(Mat &src, Mat &dst, int limit, int checkmode, int neighbormode);
    IplImage *dst;
public:
    enum MARK{
        none, BANMAXIAN, SPEAR, CHERANGREN
    };

    enum Widget::MARK mark = MARK::none;

    int spear=0; // 代表没有箭头,1左箭头，2右箭头

    explicit Widget(QWidget *parent = 0);
    ~Widget();
    bool openFile(QString path);

private slots:
    void on_pushButton_clicked();
    void on_timeout();
    void on_chooseFile_clicked();
    void on_stop();
    void on_start();
    void on_capture();

private:
    Ui::Widget *ui;
};


#endif // WIDGET_H
