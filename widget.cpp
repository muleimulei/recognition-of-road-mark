#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QScrollArea>
#include <QGridLayout>
#include "cv.h"
#include "mylabel.h"
#include "tool.h"
#include <vector>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->player->setFont(QFont("宋体",20,QFont::Bold));
    captureButton = ui->captureButton;  //截屏按钮
    connect(ui->chooseFile, SIGNAL(pressed()), this, SLOT(on_chooseFile_clicked()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    connect(ui->stop, SIGNAL(pressed()), this, SLOT(on_stop()));
    connect(ui->start, SIGNAL(pressed()), this, SLOT(on_start()));
    connect(ui->captureButton, SIGNAL(pressed()), this, SLOT(on_capture()));
    currentframeCount = 0;
    ui->tabWidget->setCurrentIndex(0);

    top = imread("top.png", IMREAD_GRAYSCALE);

//    IplImage m = (IplImage)top;
//    int s = cvCountNonZero(&m);

//    qDebug()<< s *1.0 / (top.cols * top.rows);

    right = imread("right.png", IMREAD_GRAYSCALE);
    left = imread("left.png", IMREAD_GRAYSCALE);
    l = imread("l.png", IMREAD_GRAYSCALE);
}

Widget::~Widget()
{
    delete ui;
    capture.release();
}

void Widget::on_pushButton_clicked()
{

}

void Widget::on_chooseFile_clicked()
{
    QFileDialog *filelog = new QFileDialog(this);
    filelog->setDirectory(".");
    filelog->setWindowTitle(tr("打开视频"));
    bool f = false;
    if(filelog->exec()){
        filepath = filelog->selectedFiles()[0];
        f = openFile(filepath);
    }
    if (!f) QMessageBox::warning(this,tr("error") , tr("打开失败"));
}

void Widget::on_stop()
{
    qDebug()<<"hello";
    timer.stop();
}

void Widget::on_start()
{
    timer.start();
}



bool Widget::openFile(QString path)
{
    capture.open(path.toStdString());
    if(!capture.isOpened()){
        ui->player->setText(tr("播放失败"));
        return false;
    }else{
        //获取总帧数
        totalframeCount = capture.get(CV_CAP_PROP_FRAME_COUNT);
        ui->totalrate->setText(QString::fromStdString("总帧数：%1").arg(totalframeCount));

        //获取帧率
        double r = capture.get(CV_CAP_PROP_FPS);
        ui->rate->setText(QString::fromStdString("帧率：%1").arg(r));

        long framestart = 0;
        capture.set(CV_CAP_PROP_POS_FRAMES, framestart);

        double delay = 1 / r;
        timer.start(delay * 1000);
        return true;
    }
}

void Widget::writemsg(QImage &image){
    Tools::writeMSG(image, mark, spear);
}
void Widget::judgeType(IplImage &frame){
    cv::Mat m = cvarrToMat(&frame);

    Tools::judgeSPEAR(m, right, top, left, spear, mark);
    Tools::judgeBANMAXIAN(frame, mark);
    Tools::judgeREN(m, l, mark);
}

void Widget::on_timeout(){
    if(!capture.read(currentframe)){
        ui->player->setText(QString::fromStdString("读取视频失败..."));
    }
    long cur = capture.get(CV_CAP_PROP_POS_FRAMES);
    ui->currentrate->setText(QString::fromStdString("当前帧数：%1").arg(cur));
    currentframeCount = cur; // 当前帧数
    if(currentframeCount == totalframeCount){
        capture.set(CV_CAP_PROP_POS_FRAMES, 0);
        return;
    }

    if(currentframeCount==1){
        cv::Mat m;
//        capture.set(CV_CAP_PROP_POS_FRAMES, 92);
        capture.read(m);
        capture.set(CV_CAP_PROP_POS_FRAMES, 1);

        IplImage im = IplImage(m);
        int wp = (int)((w / 820.0) * im.width);
        int hp = (int)((h / 367.0) * im.height);
        firstframe = cvCreateImage(cvSize(im.width, im.height),IPL_DEPTH_8U,1); //前一帧

        // IplImage *ret;
        // ret = getImageRect(&im,(int)((px / 820.0) * im.width), (int)((py / 367.0) * im.height), wp, hp);
        cvCvtColor(&im, firstframe, CV_BGR2GRAY);
        tempframe = cvCreateImage(cvSize(im.width, im.height),IPL_DEPTH_8U,1); // 暂存一帧
        dst = cvCreateImage(cvSize(wp, hp), IPL_DEPTH_8U, 1);
    }else{
        showdiff();
    }

    IplImage i = IplImage(currentframe);
    addtolabel(ui->player, &i);
     // 显示信息
    mark = Widget::MARK::none;
    spear = 0;
}

QImage Widget::addtolabel(QLabel *label, IplImage *im){
    QImage image = MatToImage(cvarrToMat(im));
//    QImage image = MatToImage(c);
    label->setScaledContents(true);
    QSize qs = label->rect().size();
    if(im->nChannels!=1){
        writemsg(image);
    }
    label->setPixmap(QPixmap::fromImage(image).scaled(qs));
    return image;
}

void Widget::addToall(IplImage &im){
    IplImage *dst = cvCreateImage(cvGetSize(&im),im.depth,0);
    cvLaplace(&im, dst);
    addtolabel(ui->labelall, dst);
}

void Widget::showdiff(){
    IplImage im = IplImage(currentframe);

    cvCvtColor(&im, tempframe, CV_BGR2GRAY);

    int wp = (int)((w / 820.0) * im.width);
    int hp = (int)((h / 367.0) * im.height);

//    cvCvtColor(ret,tempframe, CV_RGB2GRAY); //转化为单通道

    cvAbsDiff(tempframe, firstframe,tempframe); // 求差
    addtolabel(ui->labelall, tempframe);

    IplImage *ret;

    ret = Tools::getImageRect(tempframe,(int)((px / 820.0) * im.width), (int)((py / 367.0) * im.height), wp, hp);
//    cvShowImage("qwe", ret);

    cvSmooth(ret, ret, CV_GAUSSIAN, 3, 3); // 高斯平滑处理
    cvThreshold(ret, dst, 15, 255.0, CV_THRESH_BINARY); //二值化

    Mat p = cvarrToMat(dst);
    Mat q = Mat::zeros(Size(ret->width, ret->height), CV_8UC1);

    Tools::removesmallregion(p, q, 10, 0, 0);
    imshow("yuanlai", q);


//    Mat p = cvarrToMat(dst);
//    Mat q = Mat::zeros(Size(ret->width, ret->height), CV_8UC1);

//    Tools::removesmallregion(p, q, 25, 1, 0);
//    imshow("213", q);


//    cvSmooth(dst, dst, CV_MEDIAN, 3, 3); // 高斯平滑处理
    //cvDilate(dst, dst); // 膨胀
    //cvErode(dst, dst); // 腐蚀


    //Tools::removesmallregion(q, p, 10, 0, 0);

    IplImage m = (IplImage) q;


    addtolabel(ui->framediff, &m);
    calculategray(&m); // 计算黑色像素个数
}


void Widget::calculategray(IplImage *tempframe){ // 计算灰度和
    int s = cvCountNonZero(tempframe);
    ui->graydiff->setText(QString::fromStdString("灰度差：%1").arg(s));
    if(s > 9000){
        judgeType(*tempframe);
    }
}


QImage Widget::IplImageToImage(IplImage *image){
    uchar *data = (uchar *)image->imageData;
    return  QImage(data, image->width, image->height, QImage::Format_Indexed8);
}

QImage Widget::MatToImage(const cv::Mat& mat){
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if (mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for (int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if (mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);

        return image.rgbSwapped();
    }
    else if (mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return QImage();
    }
}

void Widget::addToPanel(QImage image)
{
//    QScrollArea *tabs = ui->scrollArea;
    static int row = 0,col = 0, width = this->rect().width() / 2;
    QLabel *l = new QLabel();
    l->setPixmap(QPixmap::fromImage(image).scaled(QSize(width,250)));
    ui->gridLayout->addWidget(l,row, col++);
    ui->gridLayoutWidget->setGeometry(QRect(0,0,this->rect().width(), (row+1) * 250));
    ui->scrollAreaWidgetContents->setMinimumHeight((row+1)*250);

    if(col == 2){
        row++;
        col=0;
    }
}

void Widget::on_capture()
{
        QDir dir("./pictures");
        if(!dir.exists()){
            dir.mkdir("pictures");
        }
        bool f = currentPic.save(QString("./pictures/%1.jpg").arg(currentframeCount));
        if(f){
            addToPanel(currentPic);
        }
}
