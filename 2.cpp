//guang liu fa

#include <iostream>
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"

#pragma comment(lib,"opencv_core2410.lib")
#pragma comment(lib,"opencv_highgui2410.lib")
#pragma comment(lib,"opencv_video2410d.lib")
#pragma comment(lib,"opencv_imgproc2410.lib")
#pragma comment(lib,"opencv_features2d2410.lib")

#define MAX_CORNERS 1000

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    //读取两幅图片
    vector<Mat> imgs,grayImgs;
    Mat img = imread("/home/meic/Documents/untitled/1.jpg");
    imgs.push_back(img);
    img = imread("/home/meic/Documents/untitled/2.jpg");
    imgs.push_back(img);

    //灰度化
    for(size_t i=0;i<imgs.size();i++){
        //复制原来的图片
        Mat temp;
        temp.create(imgs[i].rows, imgs[i].cols, CV_8UC1);

        cvtColor(imgs[i], temp, CV_RGB2GRAY);
        grayImgs.push_back(temp);
    }
    //测试是否已转化为灰度图，因为opencv里面计算光流是基于灰度图的！
    for(size_t i=0;i<imgs.size()&&i<grayImgs.size();i++){
        //imshow("origin",imgs[i]);
        //imshow("gray",grayImgs[i]);
        //waitKey(10000);
    }

    //标记待检测的特征点并显示
    vector<Point2f> point[2];
    double qualityLevel = 0.01;
    double minDistance = 10;
    /*
        void goodFeaturesToTrack( InputArray image, OutputArray corners,
                                  int maxCorners, double qualityLevel, double minDistance,
                                  InputArray mask=noArray(), int blockSize=3,
                                  bool useHarrisDetector=false, double k=0.04 )
    */
    //将imgs[0]中的检测到的角点存入point[0]中
    goodFeaturesToTrack(grayImgs[0], point[0], MAX_CORNERS, qualityLevel, minDistance);
    cout<<point[0].size()<<endl;
    /*
      void circle(CV_IN_OUT Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0);
    */
    //显示角点
    //for(size_t i= 0;i<point[0].size();i++){
       // circle(imgs[0], cvPoint(cvRound(point[0][i].x),cvRound(point[0][i].y)), 3, cvScalar(255, 0, 0), 1, CV_AA, 0);
    //}
    //imshow("detected corner", imgs[0]);
    /*
       void cv::calcOpticalFlowFarneback( InputArray _prev0, InputArray _next0,
                               OutputArray _flow0, double pyr_scale, int levels, int winsize,
                               int iterations, int poly_n, double poly_sigma, int flags )
       void line(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
    */
    //稠密光流
    Mat flow;
    calcOpticalFlowFarneback(grayImgs[0], grayImgs[1], flow, 0.5, 3, 15, 3, 5, 1.2, 0);
    cout<<flow.size()<<endl;  //对原图像每个像素都计算光流

    for(size_t y=0;y<imgs[0].rows;y+=10){
        for(size_t x=0;x<imgs[0].cols;x+=10){
            Point2f fxy = flow.at<Point2f>(y, x);
            line(imgs[0], Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)), CV_RGB(0, 255, 0), 1, 8);
        }
    }

    imshow("稠密光流：", imgs[0]);
    //稀疏光流
    TermCriteria criteria = TermCriteria(TermCriteria::COUNT|TermCriteria::EPS, 20, 0.03);
    vector<uchar> status;
    vector<float> err;

    calcOpticalFlowPyrLK(grayImgs[0], grayImgs[1], point[0], point[1], status, err, Size(15, 15), 3, criteria);

    for(size_t i=0;i<point[0].size()&&i<point[1].size();i++){
        line(imgs[1],Point(cvRound(point[0][i].x),cvRound(point[0][i].y)), Point(cvRound(point[1][i].x),
            cvRound(point[1][i].y)), cvScalar(0,50,200),1,CV_AA);
    }
    imshow("稀疏光流：", imgs[1]);
    waitKey(100000);
    return 0;
}
