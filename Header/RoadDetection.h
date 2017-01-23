/*
2017 �̰������� ĸ���� ��
CCTV ���󿡼� ���θ� ���ϴ� ���α׷��� ���� �κ�
FindLargestArea -> Contour �� ���� ū ������ �����ϴ� Contour�� ���ϴ� ���α׷�.
nonedge_area -> ������ ���� �κ��� ���� n*n �� mask�� ����� ���α׷�
roadFilter -> Ư���� Scalar ���� �ָ� float ���� �������� ���� �ִ� Scalar ������ ���͸�
Normalization -> ������ Lab �������� �ٲٰ� ��� ���� ��Ȱȭ ���ִ� ���α׷�.
*/

#include "cv.hpp" //���⿡ �ʿ��� �� �� ����
#include "opencv2/opencv.hpp" //�̷��Ը� �ϸ� �� ��� �� ���

using namespace cv;

Mat Normalization(Mat src);
Mat FindLargestArea(Mat origin, Mat cannies);
Mat nonedge_area(Mat src, float sky_rate, int window_size);
Mat roadFilter(int b, int g, int r, float magnitude, const Mat& src);
void callBackFunc(int event, int x, int y, int flags, void* userdata);