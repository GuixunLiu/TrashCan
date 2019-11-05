//只使用了opencv，通过帧差法来寻找运动物体，只是显示了运动物体，没有任何的反馈。

#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>

using namespace std;
using namespace cv;

Mat frameDiff(Mat prevFrame, Mat curFrame, Mat nextFrame);

int main()
{
	Mat frame;
	Mat preFrame, nextFrame, frame1, frame2, output;
	VideoCapture cap(1);

	if (!cap.isOpened())
	{
		cout << "error" << endl;
		waitKey(0);
		return -1;
	}

	cap >> preFrame;
	cap >> frame;
	cvtColor(preFrame, preFrame, CV_BGR2GRAY);
	cvtColor(frame, frame, CV_BGR2GRAY);
	namedWindow("res");

	namedWindow("show");
	imshow("show", frame);
	while (true)
	{
		// Capture the current frame
		cap >> nextFrame;
		cvtColor(nextFrame, nextFrame, CV_BGR2GRAY);
		imshow("show", frame);
		absdiff(nextFrame, frame, frame1);
		absdiff(frame, preFrame, frame2);
		bitwise_and(frame1, frame2, output);
		output = frameDiff(preFrame, frame, nextFrame);
		imshow("res", output);
		if (waitKey(30) >= 0)break;
		preFrame = frame;
		frame = nextFrame;
	}

	cap.release();
	// Close all windows
	destroyAllWindows();
}

Mat frameDiff(Mat prevFrame, Mat curFrame, Mat nextFrame)
{
	Mat diffFrames1, diffFrames2, output;
	// Compute absolute difference between current frame and the nextframe
	absdiff(nextFrame, curFrame, diffFrames1);
	// Compute absolute difference between current frame and the previous frame
	absdiff(curFrame, prevFrame, diffFrames2);
	// Bitwise "AnD" operation between the above two diff images
	bitwise_and(diffFrames1, diffFrames2, output);
	return output;
}
