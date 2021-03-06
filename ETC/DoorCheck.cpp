﻿#include "cv.hpp" //Header
#include "opencv2/opencv.hpp" //Header include all
#include "RoadDetection.h"
#include <deque>

using namespace std;
using namespace cv;


#define ROTATION 3 //Define how many rotation it is
#define FRAMERATE 20 //Define fps
#define QUEUE_SIZE 7
#define AV_WEIGHT float(0.2)

int number = 0;



int main(){

	int queue_sum = 0;
	float average_w = 0;

	deque<int> average_queue;

	int i = 0;

	int* min = new int(3);
	min[0] = 1000000;
	min[1] = 1000000;
	min[2] = 1000000;

	int flag = -1;
	int average_edge = 0;
	int key, frame_rate = FRAMERATE;

	VideoCapture capture("공모전\\Door2.mp4");
	//VideoCapture capture(0);

	Mat src, gray, edge, back, diff, back2, bob, bob2;
	Mat fortesting;
	Mat back_gray;

	vector<Mat> foreground(ROTATION);
	vector<Mat> background(ROTATION);
	vector<Mat> min_back(ROTATION);
	vector<Mat> temporary(ROTATION);
	vector<Mat> road_area(ROTATION);
	vector<Mat> frames(ROTATION);

	capture >> src;

	BackgroundSubtractorMOG2 *vc = new BackgroundSubtractorMOG2[ROTATION];

	//BackgroundSubtractorMOG2 vc;

	if (!src.data){
		printf("NO IMAGE\n");
		return -1;
	}

	while (capture.read(src)){

		resize(src, src, cv::Size(400, 300)); //Resize for better quality of view.

		flag++;

		if (flag == 0){
			back = src.clone();
			bob = back.clone();
			for (int i = 0; i < 3; i++){
				frames[i] = src.clone();
				temporary[i] = src.clone();
				min_back[i] = src.clone();
				road_area[i] = src.clone();
				foreground[i].create(Size(400, 300), 0);
				background[i].create(Size(400, 300), 0);
			}
			continue;
		}


		cvtColor(src, gray, CV_BGR2GRAY);
		//Sobel(gray, edge, CV_8U, 1, 0, 3);
		Canny(gray, edge, 50, 100);
		edge = edge > 128;
		cvtColor(back, back2, CV_BGR2GRAY);
		Canny(back2, back2, 50, 100);
		//Sobel(back2, back2, CV_8U, 1, 0, 3);
		back2 = back2 > 128;

		printf("%d\n", countNonZero(edge));


		///////////////////////*Queue Part, last element * 1/5 *///////////////////////

		average_queue.push_back(countNonZero(back2));

		if (average_queue.size() == QUEUE_SIZE){

			average_w = 0;
			queue_sum = 0;

			for (int i = 0; i < QUEUE_SIZE; i++){

				if (i == QUEUE_SIZE - 1){
					queue_sum += average_queue.at(i)*AV_WEIGHT;
				}
				else{
					queue_sum += average_queue.at(i);
				}

				//			printf("QUEUE[%d] = %d\n", i, average_queue.at(i));

				average_w = queue_sum / ((QUEUE_SIZE)-(1 - AV_WEIGHT));

			}

			//			printf("QUEUE_AVERAGE = %f\n", average_w);

			average_queue.pop_front();
		}

	///////////////////////////////////////////////////////////////////////////////////





		if (average_w*0.7 > countNonZero(edge) && flag >= 30){
			flag = 1;
			temporary[number] = back.clone();
			number++;
			number = number % ROTATION;
			printf("===============ROTATION %d====================\n", number + 1);
		}


		if (flag >= 20){
			vc[number](frames[number], foreground[number]);
			(vc + number)->getBackgroundImage(background[number]);
			cvtColor(background[number], back_gray, CV_BGR2GRAY);
			Canny(back_gray, back_gray, 50, 100);

			if (countNonZero(back_gray) < min[number]){
				cout << "[" << number << "]" << "Updated: " << countNonZero(back_gray) << endl;
				min[number] = countNonZero(back_gray);
				min_back[number] = background[number].clone();
				road_area[number] = nonedge_area(back_gray, 0.3, 10);

			//	imshow("BOX1", road_area[number]);

			}

		}

		//box, box3, lab_back, Scalar(mean), filter(LAB), filter(RGB), COLOR_MASK(LAB+RGB)

		char str[200];
		sprintf(str, "%d", number + 1);
		putText(src, str, Point(10, 10), 1, 1, Scalar(0, 0, 255));

		frames[number] = back.clone();

		imshow("RESULT", src);
		imshow("Canny", edge);

		//imshow("VECTORTEST1", frames[0]);
		//imshow("VECTORTEST2", frames[1]);
		//imshow("VECTORTEST3", frames[2]);

		/*imshow("Fore1", foreground[0]);
		imshow("Fore2", foreground[1]);
		imshow("Fore3", foreground[2]);
		*/
		//imshow("TESTING", fortesting);
		//imshow("Back1", background[0]);
		//imshow("Back2", background[1]);
		//imshow("Back3", background[2]);

		/*imshow("min_back[1]", min_back[0]);
		imshow("min_back[2]", min_back[1]);
		imshow("min_back[3]", min_back[2]);
*/
		key = waitKey(frame_rate);

		if (key == 27 || key == 'q')
			break;

		if (key == 32){
			if (frame_rate == FRAMERATE)
				frame_rate = 0;
			else if (frame_rate == 0)
				frame_rate = FRAMERATE;
		}
		back = src.clone();
	}
	delete[] vc;
}