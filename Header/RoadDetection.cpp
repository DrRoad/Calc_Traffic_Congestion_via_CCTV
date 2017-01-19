#include "cv.hpp" //���⿡ �ʿ��� �� �� ����
#include "opencv2/opencv.hpp" //�̷��Ը� �ϸ� �� ��� �� ���

using namespace cv;

int* bgr = (int*)malloc(3);

int* FindLargestArea(Mat origin, Mat cannies){
	Mat src;

	int i = 0, count = 0;
	int x = 0, y = 0;
	int nBlue = 0, nGreen = 0, nRed = 0;

	double maxcontour = 0;

	for (i = 0; i < 3; i++)
		bgr[i] = 0;

	vector<vector<Point>> contours;
	vector<Vec4i>hierarchy;


	src = origin.clone();

	findContours(cannies, contours, hierarchy, 2
		, CV_CHAIN_APPROX_SIMPLE);

	for (i = 0; i < contours.size(); i++){
		//
		printf("%d = %lf\n", i, contourArea(contours[i]));

		if (contourArea(contours[i])>maxcontour){
			maxcontour = contourArea(contours[i]);
			count = i;
		}

	}

	drawContours(origin, contours, count, Scalar(0, 0, 255), CV_FILLED, 8, hierarchy);

	for (x = 0; x<origin.rows; x++){
		for (y = 0; y<origin.cols; y++){
			if (origin.at<cv::Vec3b>(x, y)[0] == 0 && origin.at<cv::Vec3b>(x, y)[1] == 0 && origin.at<cv::Vec3b>(x, y)[2] == 255){
				nBlue = src.at<cv::Vec3b>(x, y - 1)[0];
				nGreen = src.at<cv::Vec3b>(x, y - 1)[1];
				nRed = src.at<cv::Vec3b>(x, y - 1)[2];
			}
		}
	}

	bgr[0] = nBlue;
	bgr[1] = nGreen;
	bgr[2] = nRed;

	//
	printf("In Function: Blue = %d, Green = %d, Red = %d\n", bgr[0], bgr[1], bgr[2]);


	return bgr;
}

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

Mat roadFilter(int b, int g, int r, float magnitude, const Mat&src) {
	int B, G, R;
	assert(src.type() == CV_8UC3);
	Mat filter;

	// To
	if ((magnitude*b)>255)   B = 255;
	else   B = magnitude * b;
	if ((magnitude*g) > 255)   G = 255;
	else   G = magnitude * g;
	if ((magnitude*r) > 255) R = 255;
	else   R = magnitude * r;

	// From
	b = b / magnitude;
	g = g / magnitude;
	r = r / magnitude;

	//mask
	inRange(src, Scalar(b, g, r), Scalar(B, G, R), filter); //Threshold the image
	dilate(filter, filter, getStructuringElement(MORPH_RECT, Size(5, 5)));
	erode(filter, filter, getStructuringElement(MORPH_RECT, Size(5, 5)));
	erode(filter, filter, getStructuringElement(MORPH_RECT, Size(5, 5)));
	dilate(filter, filter, getStructuringElement(MORPH_RECT, Size(5, 5)));
	
	int* bgr = (int*)malloc(3);
	
	return filter;
}
