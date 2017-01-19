#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <time.h>
#include "nonedge_area.cpp"

using namespace std;
using namespace cv;

Mat nonedge_area(Mat src, float sky_rate, int window_size) {
	/*
	Mat src :  ���� ����(����ó����->2��ȭ�������� ��ȯ�� �����̾����.
	float sky_rate : �ϴÿ� �ش��ϴ� ���� (ex/ 0.3 : ���� 30%�� �����Ѵ�)
	int window_size : �������� ũ�� : �������� �����ϰ� �˻�.
	*/

	int i, i2 = 0;
	int j, j2 = 0;
	int src_height, src_width;

	src_height = src.rows;
	src_width = src.cols;

	Mat window;
	Mat output(src_height, src_width, src.type(), Scalar(0));

	float m_height = src_height * sky_rate;

	for (i = m_height; i + window_size <= src_height; i = i + window_size) {
		if (i + window_size >= src_height)
			i2 = src_height;
		else
			i2 = i + window_size;

		for (j = 0; j + window_size <= src_width; j = j + window_size) {
			if (j + window_size >= src_width)
				j2 = src_width;
			else
				j2 = j + window_size;

			window = src(Range(i, i2), Range(j, j2));
			if (sum(window) == Scalar(0))
				output(Range(i, i2), Range(j, j2)) = Scalar(255);
		}
	}
	return output;

}

int main(void) {
	Mat frame, poly;
	Mat back, fore, canny, gray;

	//VideoCapture cap("example2.avi");
	VideoCapture cap("example.mp4");
	int start = 0;
	BackgroundSubtractorMOG2 bg;

	while (1) {
		cap >> frame;

		if (!frame.data) {
			cout << "end" << endl;
			break;
		}
		//foreground ���
		bg(frame, fore);
		bg.getBackgroundImage(back);

		erode(fore, fore, Mat());
		dilate(fore, fore, Mat());
/*
		if (start == 0) {
			oldfore = fore.clone();
			start = 1;
		}
		else
			oldfore += fore.clone(); //oldfore�� �׳� ������ ������ ������ ��� ���ϴ� ����.
*/
		cvtColor(back, gray, CV_BGR2GRAY);
		Canny(gray, canny, 10, 30, 3);

		threshold(canny, canny, 127, 255, CV_THRESH_BINARY);
	
		poly = nonedge_area(canny, 0.3, 10);

		imshow("poly", poly);
		imshow("Frame", frame);
		imshow("Canny", canny);
		imshow("Background", back);
		
		if (waitKey(1) >= 0) break;
	}
	return 0;
}


