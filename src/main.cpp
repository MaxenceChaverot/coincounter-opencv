#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;

int main(int argc, char* argv[])
{
	Mat src;

	/*Parsing Command Line Argument*/

	if(argc < 2){
		src = imread("img/pieces.jpg", 1);
	}
	else{
		src = imread(argv[1]);
	}
	/******************************/


	Mat src_gray;

	// Convert it to gray
	cvtColor(src, src_gray, CV_BGR2GRAY);

	// Apply the Hough Transform to find the circles
	// CV_HOUGH_GRADIENT : detection method to use (currently, the only implemented one)
	// 1 : inverse ratio of the accumulator resolution to the image resolution
	// src_gray.rows/8 : minimum distance between the centers of the detected circles
	// 200 : the higher threshold of the two passed to the Canny() edge detector (the lower one is twice smaller)
	// 100 : the accumulator threshold for the circle centers at the detection stage. The smaller it is, the more false circles may be detected
	// 0 : Minimum circle radius
	// 0 : Maximum circle radius

	std::vector<Vec3f> circles;
	std::vector<Mat> extractedCircles;

	HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 100, 0, 0);

	std::cout<<"Nombre de pièces trouvées : "<<circles.size()<<std::endl;

	// Draw the circles detected and Extract them
	for( size_t i = 0; i < circles.size(); i++ )
	{
		//DrawCircle
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
		// circle outline
		circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );


		//Extract Circle
		//get the Rect containing the circle:
		Rect r(center.x-radius, center.y-radius, radius*2,radius*2);

		// obtain the image ROI:
		Mat roi(src, r);

		// make a black mask, same size:
		Mat mask(roi.size(), roi.type(), Scalar::all(0));
		// with a white, filled circle in it:
		circle(mask, Point(radius,radius), radius, Scalar::all(255), -1);

		// combine roi & mask:
		Mat circleCropped = roi & mask;

		extractedCircles.push_back(circleCropped);
	}

	// Show original Image
	namedWindow("Original image with coins", WINDOW_NORMAL);
	resizeWindow("Original image with coins", src.cols, src.rows);
	imshow("Original image with coins", src);


	// Show results for DEBUG

	for(size_t i = 0; i < circles.size(); i++)
	{
		string nameWin = string("Extracted Circle  %d", i+1);
		namedWindow(nameWin, WINDOW_NORMAL);
		resizeWindow(nameWin, extractedCircles[i].cols, extractedCircles[i].rows);
		imshow(nameWin, extractedCircles[i]);
	}

	waitKey(0);

	return 0;
}

