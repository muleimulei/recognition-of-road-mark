#ifndef TOOL_H
#define TOOL_H
#include "opencv/highgui.h"
#include "widget.h"
#include <QPainter>
#include <vector>
#include "mylabel.h"
#include <iostream>
#include "cv.h"
#define LEVEL 35
#define SPAN 20

class Tools{
public:
    static bool f;
    static void judgeBANMAXIAN(IplImage &frame, Widget::MARK &mark){
        if(mark==Widget::MARK::SPEAR) return;
        int height = (int)(frame.height / 4);
        uchar *d;
        int h = height;
        for(int i=0; i<3; i+=1){
            int lw = 0, lb = 0;
            bool f = false;
            int cw=0, cb=0;

            for(int j=0; j<frame.width; j++){
                d = (uchar *) (frame.imageData + h * frame.widthStep + j);
                if(*d + 0 == 255){
                    if(lw!=0 &&abs(lw - lb)<10){
                        f = true;
                        break;
                    }
                    cw +=1;
                    if (cw>LEVEL) {
                        lw=cw;
                    }
                    if(lb==0){
                        cb=0;
                    }
                }else{
                    cb+=1;
                    if(cb>LEVEL){
                        lb = cb;
                    }
                    if(lw==0){
                        cw=0;
                    }
                }
            }
//            qDebug()<<lw<<" "<<lb;
            if(f){
                mark = Widget::MARK::BANMAXIAN;
                return;
            }
            h += height;
        }
        mark = Widget::MARK::none;
    }
    static void writeMSG(QImage &image, Widget::MARK &mark, int &spear){
        QPainter painter(&image);
        // 改变画笔和字体
        QPen pen = painter.pen();
        pen.setColor(Qt::yellow);
        QFont font = painter.font();
        font.setBold(true);
        font.setPixelSize(80);
        painter.setPen(pen);
        painter.setFont(font);

        if(mark == Widget::MARK::none){
            painter.drawText(100,100, "未检测到标线...");
        }else if(mark == Widget::MARK::BANMAXIAN){
            painter.drawText(100,100, "检测到斑马线...");
        }else if(mark == Widget::MARK::SPEAR){
            if(spear == 2 && f){
                painter.drawText(100, 100, "检测到箭头...");
                painter.drawText(100, 200, "直行右转箭头...");
            }else if(spear == 1 &&f){
                painter.drawText(100, 100, "检测到箭头...");
                painter.drawText(100, 200, "直行左转箭头...");
            }
        }else if(mark == Widget::MARK::CHERANGREN){
            painter.drawText(100, 100, "检测到文字...");
            painter.drawText(100, 200, "车让人...");
        }
    }

    static void judgeSPEAR(cv::Mat src, Mat &right, Mat &top, Mat &left, int &spear, Widget::MARK &mark){
//        cv::Mat  leftspear, topspear, rightspear;
//        imshow("top", top);
        int result_cols, result_rows;

        //--------------------------------------------

        // 识别右箭头
        result_cols = src.cols - right.cols + 1;
        result_rows = src.rows - right.rows + 1;
        Mat result = cv::Mat(result_cols, result_rows, CV_32FC1);
        cv::matchTemplate(src, right, result, TM_SQDIFF_NORMED);

        double minVal, maxVal;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

        // 截取图像
        Rect rect = Rect(minLoc, cv::Size(right.cols, right.rows));

        Mat p = (Mat(src, rect)).clone(); //将箭头截取下来
        Mat dst = Mat::zeros(Size(p.cols, p.rows), CV_8UC1);
        cv::rectangle(src, cv::Rect(minLoc, cv::Size(right.cols, right.rows) ), Scalar(255, 0, 0), 2, 8, 0 );
//        removesmallregion(p, dst, 15, 0, 0);
        judgeSpear(p, spear, mark);
        //---------------------------------------------

        // 识别上箭头
        result_cols = src.cols - top.cols + 1;
        result_rows = src.rows - top.rows + 1;
        result = cv::Mat(result_cols, result_rows, CV_32FC1);
        cv::matchTemplate(src, top, result, TM_SQDIFF_NORMED);

//        double minVal, maxVal;
//        cv::Point minLoc, maxLoc;
        cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

        // 截取图像
        Rect rect1 = Rect(minLoc, cv::Size(top.cols, top.rows));

        Mat p1 = (Mat(src, rect1)).clone(); //将箭头截取下来
        Mat dst1 = Mat::zeros(Size(p1.cols, p1.rows), CV_8UC1);

        cv::rectangle(src, cv::Rect(minLoc, cv::Size(top.cols, top.rows) ), Scalar(255, 0, 0), 2, 2, 0 );

//        removesmallregion(p1, dst1, 15, 0, 0);

        f = judgeTopSpear(p1);
//        qDebug()<<f;
//        imshow("gray", src);
    }

    static bool judgeTopSpear(Mat &p){
//        qDebug()<<"width: "<<p.cols <<" Height: "<<p.rows; // 98 49
        IplImage m = (IplImage)p;
        int s = cvCountNonZero(&m);
        if((s * 1.0 / (p.rows * p.cols) - 0.5)>0.05){
//            qDebug()<<"false: 1122";
            return false;
        }
        int width = p.cols, height = p.rows;
        vector<int> v;
        for(int i = 0; i<height;i++){ //得到 每行的白色像素和
            int sum=0;
            for(int j=0; j<width;j++){
                uchar *data = p.ptr<uchar>(i, j);
                if(*data + 0 ==255){
                    sum++;
                }
            }
            v.push_back(sum);
        }
        // 计算每列的白色像素和
        vector<int > v1;
        for(int i = 0; i<width; i++){
            int sum=0;
            for(int j = 0; j<height;j++){
                uchar *data = p.ptr<uchar>(j, i);
                if( *data + 0 == 255 ){
                    sum++;
                }
            }
            v1.push_back(sum);
        }
        int mm, ss;
        int l = getsmallandbig(v1, mm, ss); //得到列的最大值
        if(l<SPAN && abs(width- l) <SPAN){
            return false;
        }

        int e = 0;
        int max, min;
        int r = getsmallandbig(v, max, min);
        for(int i=1; i<r;i++){
            if(v[i]<v[i-1]){
                e++;
            }
            if(v[i]>v[i-1] && v[i]-v[i-1] > 10){
                return false;
            }
        }
        if(e>5){
            return false;
        }else{

            int pos = getpos(v, min);
            if( (pos>5) && min> 3){
                return false;
            }else{
                int top = r >= 1? r-1: r;
                int bottom = height - r >1? r+1: height-1;
                int t = 0;
                for(int i = bottom; i<height-1;i++){
                    t += v[i];
                }
                t /= (height - bottom);
                if(abs(v[top] - v[r]) < 3 && abs(v[r] - t) > 10){
                    return true;
                }
            }
        }
    }
    static int getsmallandbig(vector<int> &v, int &max1, int &min){ // 得到 数组的最大值与最小值
        int q =-1;
        max1 = INT_MIN;
        min = INT_MAX;
        for(int i=0; i<v.size();i++){
            if(v[i] > max1){
                q = i;
                max1 = v[i];
            }
            if(v[i]<min){
                min = v[i];
            }
        }
        return q; // 返回最大值的下标
    }
    static int getpos(vector<int> &v, int val){
        for(int i = v.size()-1; i>=0;i--){
            if(val==v[i]){
                return i;
            }
        }
    }
    static void judgeSpear(Mat &p, int &spear, Widget::MARK &mark){
        IplImage m =(IplImage)p;
        int sum = cvCountNonZero(&m);
        int cols = p.cols, rows = p.rows;
        if( abs(sum * 1.0 / (cols * rows) - 0.5) > 0.05){
            mark = Widget::MARK::none;
            spear = 0;
            return;
        }
        vector<int> v1, v2;
        for(int i=0; i <rows; i++){
            int sum=0;
            for(int j = 0; j< cols; j++){
                uchar *data = p.ptr<uchar>(i, j);
                if(*data + 0 == 255){
                    sum++;
                }
            }
            if(i>0 &&abs(sum - v1[i-1])>SPAN){
                v1.push_back(v1[i-1]);
                continue;
            }
            v1.push_back(sum);
        }

        for(int i = 0; i<cols;i++){
            int sum = 0;
            for(int j = 0; j<rows; j++){
                uchar *data = p.ptr<uchar>(j, i);
                if(*data + 0 ==255){
                    sum++;
                }
            }
            v2.push_back(sum);
        }

        int max1, max2;
        int min1, min2;
        int h = getsmallandbig(v1, max1, min1), l = getsmallandbig(v2, max2, min2); // 得到行列的最大值
        int pos = getpos(v2, min2);

//        qDebug()<<"h :"<<h <<"  l:"<<l<<" pos:"<<pos<<" max1:"<<max1<<" max2:"<<max2;

        if(abs(rows/2 - h) < 10 && abs(cols-pos)<10 && abs(rows - max2)<30){
            mark = Widget::MARK::SPEAR;
            spear = 2;
            return;
        }else if(abs(rows/2 - h) < 10 && pos<10 && abs(rows - max2)<30){
            mark = Widget::MARK::SPEAR;
            spear = 1;
            return;
        }else{
            mark = Widget::MARK::none;
            spear = 0;
            return;
        }
    }


    static void judgeREN(cv::Mat &src, cv::Mat &ren, Widget::MARK &mark ){
        int result_cols, result_rows;
        // 识别文字
        result_cols = src.cols - ren.cols + 1;
        result_rows = src.rows - ren.rows + 1;
        Mat result = cv::Mat(result_cols, result_rows, CV_32FC1);
        cv::matchTemplate(src, ren, result, TM_SQDIFF_NORMED);
        double minVal, maxVal;
        cv::Point minLoc, maxLoc;
        cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
        // 截取图像
        Rect rect = Rect(minLoc, cv::Size(ren.cols, ren.rows));
        Mat p = (Mat(src, rect)).clone(); //将文字截取下来
        cv::rectangle(src, cv::Rect(minLoc, cv::Size(ren.cols, ren.rows) ), Scalar(255, 0, 0), 2, 2, 0 );
        imshow("gray", src);
        int start = 0, end = 0;
        int w = 0;
        for(int i = 0; i<p.cols; i++){
            uchar * c = p.ptr(15, i);
            if(*c + 0 == 255){
                if(start==0){
                    start = i;
                }
                w+=1;
            }
        }
        end = start+w;

//        qDebug()<<start<<" "<<end;
        int mid = (int)((start + end) / 2);
        if(abs(40 - start) > 10 || abs(60 - end)> 10){
//            mark = Widget::MARK::none;
            return;
        }

        cv::Mat left = (Mat(src, Rect(0,0, mid, p.rows))).clone(); // 按中心轴分开
        cv::Mat right = (Mat(src, Rect(mid, 0, p.cols-mid, p.rows))).clone();

        IplImage lef = (IplImage)left;
        IplImage rig = (IplImage) right;
        int num1 = cvCountNonZero(&lef);
        int num2 = cvCountNonZero(&rig);


//        qDebug()<<num1<<" "<<num2;

        if(abs(num1-num2)> 100){
//            mark = Widget::MARK::none;
            return;
        }else{
            qDebug()<<"车让人。。。。";
            mark = Widget::MARK::CHERANGREN;
        }

    }

    static IplImage *getImageRect(IplImage *ori, const int px, const int py, const int w, const int h){
        // 从图像ori中提取一块子图象ret
        IplImage *result;
        CvSize size;
        size.width = w;
        size.height = h;
        result = cvCreateImage(size, ori->depth, ori->nChannels);
        // 从图像中提取子图像
        cvSetImageROI(ori, cvRect(px, py, w, h));
        cvCopy(ori, result);
        cvResetImageROI(ori);
        return result;
    }
    // checkmode : 0 代表去除黑区域，1代表去除白区域； neighbormode: 0代表4邻域，1代表8邻域
    static void removesmallregion(Mat &src, Mat &dst, int limit = 50, int checkmode = 1, int neighbormode=0){
        cv::Mat &srcmat = src;
        int removeCount = 0; // 记录除去的个数
        // 记录每个像素点检查状态的标签，0代表未检查，1代表正在检查，2代表检查不合格（需要反转颜色），3代表检查合格或不需要检查
        Mat pointlabel = Mat::zeros(srcmat.size(), CV_8UC1);
        if(checkmode==1){
    //        qDebug()<<"Mode : 去除小区域";
            for(int i=0; i<srcmat.rows; i++){
                uchar *idata = srcmat.ptr<uchar>(i);
                uchar *ilabel = pointlabel.ptr<uchar>(i);
                for(int j=0; j<srcmat.cols; j++){
                    if(idata[j]<10){
                        ilabel[j] = 3;
                    }
                }
            }
        }else{
    //        qDebug()<<"Mode: 去除孔洞。";
            for(int i=0;i<srcmat.rows; i++){
                uchar *idata = srcmat.ptr<uchar>(i);
                uchar *ilabel = pointlabel.ptr<uchar>(i);
                for(int j=0;j<srcmat.cols;j++){
                    if(idata[j]>10){
                        ilabel[j]=3;
                    }
                }
            }
        }

        vector<Point> neighborpos;
        neighborpos.push_back(Point(-1, 0));
        neighborpos.push_back(Point(1, 0));
        neighborpos.push_back(Point(0, -1));
        neighborpos.push_back(Point(0, 1));
        if(neighbormode==1){
    //        qDebug()<<"Neighbor Mode : 8邻域"<<endl;
            neighborpos.push_back(Point(-1, -1));
            neighborpos.push_back(Point(-1, 1));
            neighborpos.push_back(Point(1, -1));
            neighborpos.push_back(Point(1,1));
        }else{
    //        qDebug()<<"Neighbor mode: 4邻域";
        }
        int neighborcount = 4 + 4 * neighbormode;
        int curX = 0, curY = 0;

        // 开始检测
        for(int i=0; i<srcmat.rows; i++){
            uchar *ilabel = pointlabel.ptr<uchar>(i);
            for(int j=0; j<srcmat.cols; j++){
                if(ilabel[j]==0){
                    // 开始在该点处检查
                    vector<Point> growbuffer; // 堆栈，用于存储生长点
                    growbuffer.push_back(Point(j, i));
                    pointlabel.at<uchar>(i, j) = 1;
                    int checkresult = 0; // 用于判断结果（是否超出大小），0为未超出，1为超出

                    for(int z=0; z<growbuffer.size();z++){
                        for(int q = 0; q<neighborcount; q++){
                            curX = growbuffer.at(z).x + neighborpos.at(q).x;
                            curY = growbuffer.at(z).y + neighborpos.at(q).y;
                            if(curX >= 0 && curX < srcmat.cols && curY >= 0 && curY < srcmat.rows){ // 防止越界
                                if(pointlabel.at<uchar>(curY, curX)==0){
                                    growbuffer.push_back(Point(curX, curY)); // 临界点加入buffer
                                    pointlabel.at<uchar>(curY, curX) = 1; // 更新邻域点的检查标签，避免重复检查
                                }
                            }
                        }
                    }
                    if(growbuffer.size()>limit) checkresult = 2;
                      // 判断结果（是否超出限定的大小），1为未超出，2为超出
                    else {
                        checkresult = 1;
                        removeCount ++;
                    }
                    for(int z=0; z < growbuffer.size(); z++){
                        curX = growbuffer.at(z).x;
                        curY = growbuffer.at(z).y;
                        pointlabel.at<uchar> (curY, curX) += checkresult;
                    }
                    // 结束该点处的检查
                }
            }
        }

        checkmode = 255 * (1 - checkmode);
        //开始反转面积过小的区域

        for(int i=0; i<srcmat.rows; i++){
            uchar *idata = srcmat.ptr<uchar>(i);
            uchar *idstdata = dst.ptr<uchar>(i);
            uchar *ilabel = pointlabel.ptr<uchar>(i);

            for(int j=0; j<srcmat.cols; j++){
                if(ilabel[j]==2){
                    idstdata[j] = checkmode;
                }else if(ilabel[j]==3){
                    idstdata[j] = idata[j];
                }
            }
        }
    }
};

bool Tools::f = false;
#endif // TOOL_H
