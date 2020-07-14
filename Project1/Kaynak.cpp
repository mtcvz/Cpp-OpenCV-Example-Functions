#include <iostream>
#include <cstdlib>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void openImage();
void openVideo();
void openWebcam();
void findRowCol();
void resizeImage();
void colorfulImage();
void convertGray();
void convertBinary();
void convertReverse();
void resminKanallari();
void pixels();
void grayPixels();
void kontrast();
Mat convertKontrast(Mat,int,float);
void brightness();
Mat brightnessManual(Mat, int, float, float);
	

int main() {
	
	Mat img, img3;
	img = imread("araba.jpg");
	imshow("orj", img);

	img3 = brightnessManual(img, -1, 1, -100);
	imshow("new", img3);

	waitKey(0);

	//brightness();
}

void openImage() {
	Mat img;
	img = imread("araba.jpg");

	namedWindow("Resim Penceresi", CV_WINDOW_AUTOSIZE);

	if (img.empty()) {
		cout << "Resim yuklemedi." << endl;
		exit(1);
	}

	imshow("Resim Penceresi", img);
	waitKey(0);
	destroyWindow("Resim Penceresi");
}

void openVideo() {

	VideoCapture vid("video.mp4");
	namedWindow("Video Penceresi", CV_WINDOW_AUTOSIZE);

	if (!vid.isOpened()) {
		cout << "Video acilamadi." << endl;
		exit(1);
	}

	float fps = vid.get(CAP_PROP_FPS);
	cout << fps << endl;

	bool frame;

	while (true) {
		Mat video;
		
		frame=vid.read(video);

		if (waitKey(30) == 27 || !frame) {
			destroyWindow("Video Penceresi");
			break;
		}

		imshow("Video Penceresi", video); 
	}
}

void openWebcam() {

	VideoCapture cam(0);
	
	if (!cam.open(0)) exit(1);

	while (true) {
		Mat frame;
		cam >> frame;

		if (waitKey(30) == 27 || frame.empty()) break;
		
		imshow("Webcam", frame);
	}

}

void findRowCol() {

	Mat img = imread("araba.jpg");

	int row, col;
	
	row = img.rows;
	col = img.cols;
	cout << row << endl << col;
}

void resizeImage() {

	Mat newsize;
	
	Mat img1 = imread("temp.png");
	Mat img2 = imread("araba2.jpg");

	resize(img2, newsize, Size(img1.cols,img1.rows));

	namedWindow("resim1", CV_WINDOW_AUTOSIZE);
	namedWindow("resim2", CV_WINDOW_AUTOSIZE);
	namedWindow("resim3", CV_WINDOW_AUTOSIZE);

	imshow("resim1", img1);
	imshow("resim2", img2);
	imshow("resim3", newsize);

	waitKey(0);
}

void colorfulImage() {

	// Mat imgGray(240,240,CV_8UC1, Scalar(128));
	Mat img(240, 240, CV_8UC3, Scalar(255,255,255));

	namedWindow("Resim", CV_WINDOW_AUTOSIZE);
	imshow("Resim", img);
	waitKey(0);
}

void convertGray() {

	Mat img = imread("araba.jpg");
	Mat imgGray;
	
	cvtColor(img, imgGray, CV_BGR2GRAY);
	
	imshow("Resim", imgGray);
	waitKey(0);
}

void convertBinary() {

	Mat img,gray,binary;
	img = imread("araba.jpg");

	cvtColor(img, gray, CV_BGR2GRAY);

	threshold(gray, binary,30 , 255, THRESH_BINARY); // 30 : esik degeri. 30'un alti 0, ustu 255 gibi. 
													 // 255 : esik degerinin ustundekilere verilecek deger.
	imshow("gri", gray);
	imshow("bin", binary);
	waitKey(0);
}

void convertReverse() {
	Mat img = imread("araba.jpg");
	Mat newimg;

	bitwise_not(img, newimg);
	imshow("first", img);
	imshow("sec", newimg);

	waitKey(0);
}

void resminKanallari() {

	Mat img = imread("araba.jpg");
	imshow("first", img);

	Mat rgbkanal[3];
	
	split(img, rgbkanal); //kanallarina ayirir

	imshow("red", rgbkanal[0]);
	imshow("green", rgbkanal[1]);
	imshow("blue", rgbkanal[2]);
	
	vector<Mat> kanallar;
	Mat image;
	
	for (int i = 0; i < 3; i++)
		kanallar.push_back(rgbkanal[i]);
	
	merge(kanallar, image);

	imshow("new", image);

	
	waitKey(0);
}

void pixels() {

	Mat img = imread("araba.jpg");

	int red, green, blue;

	blue = img.at<Vec3b>(100, 120)[0];
	green = img.at<Vec3b>(100, 120)[1];
	red = img.at<Vec3b>(100, 120)[2];

	cout << "Blue:" << blue << " Green:" << green << " Red:" << red;
}

void grayPixels() {
	Mat img = imread("araba.jpg");

	resize(img, img, Size(500, 500));

	cvtColor(img,img, CV_BGR2GRAY);

	int pixelValue = img.at<uchar>(100, 100);

	cout << "Pixel Value:" << pixelValue;

	imshow("resim", img);
	waitKey(0);
}

void kontrast() {
	// en karanlik pixel ile en aydinlik pixelin farkidir.
	// fark ne kadar yuksekse resmin renk skalasi o kadar genistir.
	// resmin tum pixellerini k katsayisi ile carparak farki azaltiriz.
	// konstrast katsayisi buyudukce resim beyaza yaklasir
	// kontrast katsayisi dustukce resim siyaha yaklasir

	Mat img = imread("araba.jpg");
	Mat newHigh, newLow;

	img.convertTo(newHigh,-1,2,0); // -1 degeri ve biti ayni kalmasi icin
								  // 2 degeri k katsayisi
								  // 0 ise her piksele yapilan ekleme

	img.convertTo(newLow, -1, 0.2, 0);
	
	imshow("1", img);
	imshow("2", newHigh);
	imshow("3", newLow);

	waitKey(0);
}

Mat convertKontrast(Mat input,int depth,float alpha) {

	if (depth == -1) {

		for (int i = 0; i < input.rows; i++) {
			for (int j = 0; j < input.cols; j++) {

				if (input.at<Vec3b>(i, j)[0] * alpha > 255)
					input.at<Vec3b>(i, j)[0] = 255;
				else
					input.at<Vec3b>(i, j)[0] *= alpha;

				if (input.at<Vec3b>(i, j)[1] * alpha > 255)
					input.at<Vec3b>(i, j)[1] = 255;
				else
					input.at<Vec3b>(i, j)[1] *= alpha;
			
				if (input.at<Vec3b>(i, j)[2] * alpha > 255)
					input.at<Vec3b>(i, j)[2] = 255;
				else
					input.at<Vec3b>(i, j)[2] *= alpha;
			}
		}
	}

	if (depth == 1) {
		
		cvtColor(input, input, CV_BGR2GRAY);

		for (int i = 0; i < input.rows; i++) {
			for (int j = 0; j < input.cols; j++) {

				if (input.at<uchar>(i, j) * alpha > 255)
					input.at<uchar>(i, j) = 255;
				else
					input.at<uchar>(i, j) *= alpha;
			}
		}
	}

	return input;
}

void brightness() {

	Mat img = imread("araba.jpg");
	Mat img2, img3;

	img.convertTo(img2, -1,1,100);	// -1 degeri giris resminin ayni kalmasi icin
	img.convertTo(img3, -1, 1, -100);	// 1 katsayi. 100 de piksellere eklenen deger.

	imshow("1", img2);
	imshow("2", img3);

	waitKey(0);
}

Mat brightnessManual(Mat input, int depth, float alpha, float beta) {

	if (depth == -1) {

		for (int i = 0; i < input.rows; i++) {
			for (int j = 0; j < input.cols; j++) {
				for (int k = 0; k < 3; k++) {
					if (input.at<Vec3b>(i, j)[k] * alpha + beta > 255)
						input.at<Vec3b>(i, j)[k] = 255;
					else if (input.at<Vec3b>(i, j)[k] * alpha + beta < 0)
						input.at<Vec3b>(i, j)[k] = 0;
					else
						input.at<Vec3b>(i, j)[k] = input.at<Vec3b>(i, j)[k] * alpha + beta;

				}
			}
		}
	}

	if(depth == 1){
	
		cvtColor(input, input, CV_BGR2GRAY);

		for (int i = 0; i < input.rows; i++) {
			for (int j = 0; j < input.cols; j++) {
				if (input.at<uchar>(i, j) * alpha + beta > 255)
					input.at<uchar>(i, j) = 255;
				else if (input.at<uchar>(i, j) * alpha + beta < 0)
					input.at<uchar>(i, j) = 0;
				else
					input.at<uchar>(i, j) = input.at<uchar>(i, j) * alpha + beta;
			}
		}
	}
	return input;
}