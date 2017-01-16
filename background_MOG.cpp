#include "cv.hpp" //���⿡ �ʿ��� �� �� ����
#include "opencv2/opencv.hpp" //�̷��Ը� �ϸ� �� ��� �� ���

using namespace std;
using namespace cv;

int main(){
	Mat src, weight, back;
	VideoCapture capture("C:\\Users\\Administrator\\Desktop\\Study\\4�г�\\������\\OpenCV\\traffic.mp4");
	int key;
	int frame_rate = 30;

	BackgroundSubtractorMOG fgbg;

	capture >> src;

	while (capture.read(src)){
		fgbg(src, back, (0.1));

		imshow("ORG", src);
		imshow("BACK", back);

		key = waitKey(frame_rate);

		if (key == 27)
			break;
	}
}