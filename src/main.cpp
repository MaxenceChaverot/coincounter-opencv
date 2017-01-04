#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <stdio.h>

#include "bddimage.hpp"
#include "circlesdetection.hpp"

using namespace cv;

static void help();
void parseParameters(CommandLineParser& parser, std::string& im_path,std::string& bdd_path,std::string& ip_num,int& method_num);

int main(int argc, char* argv[])
{
	// Help message
	help();

	//Command Line Parameters Defaults
	std::string im_path = "img/piecestest1.jpg",bdd_path="img/bdd/";
	std::string ip_num="ORB"; //ORB Detector used by default
	int method_num=1; // Circles detection without pretreatment is used by default

	//Set parameters to Input value
	const char* keys = "{path ||}{path_bbd ||}{method_num ||}{ip_num ||}";

	CommandLineParser parser(argc, argv, keys);
	parseParameters(parser, im_path,bdd_path,ip_num,method_num);

	Mat src = imread(im_path);
	int detection_number_method = method_num; 
	/****************************************************************/
	/* Circles Detection */   
	/****************************************************************/

	CirclesDetection circles_Detection(src);
	circles_Detection.DetectCircles(detection_number_method);
	std::vector<Mat> extractedCircles = circles_Detection.GetOutputExtractedCircles();
	Mat img_with_circles_drawn = circles_Detection.GetOutputImage();

	std::cout<<"Number of coins finded : "<<extractedCircles.size()<<std::endl;

	/* Show original Image with circles detected drawn */

	namedWindow("Original image with coins", WINDOW_NORMAL);
	resizeWindow("Original image with coins", img_with_circles_drawn.cols, img_with_circles_drawn.rows);
	imshow("Original image with coins", img_with_circles_drawn);

	/*
	// Show results for DEBUG
	for(size_t i = 0; i < extractedCircles.size(); i++)
	{
	std::ostringstream s;
	s << "Extracted Circle "<< i+1;
	std::string nameWin(s.str());
	namedWindow(nameWin, WINDOW_NORMAL);
	resizeWindow(nameWin, extractedCircles[i].cols, extractedCircles[i].rows);
	imshow(nameWin, extractedCircles[i]);
	}*/

	/****************************************************************/
	/* Comparison with the database */
	/****************************************************************/

	Mat img_bdd = imread("img/bdd/1_cent.png");
	Comparator orb_comparator("ORB");

	// Find the keypoints
	std::vector<KeyPoint> keypoints_to_match = orb_comparator.findKeyPoints(src);
	std::vector<KeyPoint> keypoints_bdd = orb_comparator.findKeyPoints(img_bdd);

	// Find the descriptors
	Mat descriptors_to_match = orb_comparator.findDescriptors(src, keypoints_to_match);
	Mat descriptors_bdd = orb_comparator.findDescriptors(img_bdd, keypoints_bdd);

	// Find the good matches
	std::vector<DMatch> good_matches = orb_comparator.match(descriptors_to_match, descriptors_bdd);

	// Draw matches
	Mat output = orb_comparator.drawOutputMatches(src, img_bdd, keypoints_to_match, keypoints_bdd, good_matches);

	// Get the homography between two images
	//Mat H = orb_comparator.GetHomography(src, img_bdd);

	namedWindow("Matches", WINDOW_NORMAL);
	resizeWindow("Matches", output.cols, output.rows);
	imshow("Matches", output);

	waitKey(0);

	return 0;
}

static void help()
{
	std::cout <<
		"\nThis program demonstrates a coins counting program using OpenCV. \n"
		"You can use two different methods for the circles detection :\n"
		" - without image pretreatment (method_num = 1) (used by default)\n"
		" - with image pretreatment (Canny filtering, Smoothing, Dilatation, Erosion)  (method_num = 2)\n"
		"This program has four arguments defined below :\n"
		"-path=<the path of the source image>\n"
		"-path_bbd=<the path of the database folder>\n"
		"-method_num=<the circles detection method number (1 or 2) >\n"
		"-ip_num=<the method used to chose the interest points during the comparison : ORB or MSER> chosen  \n" << std::endl;
}

void parseParameters(CommandLineParser& parser, std::string& im_path,std::string& bdd_path,std::string& ip_num,int& method_num){

	/****************************************************************/
	/* Parsing Command Line Argument */
	/****************************************************************/

	// path = the path of the source image
	// method_num = if 0, the detection is done without pretreatment, if 1, the detection is done with pretreatment


	// Path of the source image
	std::string str = parser.get<String>("path");
	if(!str.empty())
	{
		im_path = str;
		std::cout<<"path argument : "<<im_path<<std::endl;
	}

	str = parser.get<String>("path_bdd");
	if(!str.empty())
	{
		bdd_path = str;
		std::cout<<"path argument : "<<im_path<<std::endl;
	}

	str = parser.get<String>("ip_num");
	if (str.compare("ORB")==0 || str.compare("MSER")==0){
		ip_num = str;
		std::cout<<"Algorithm Argument : "<<ip_num<<std::endl;
	}

	// Detection method (with or without pretreatment)
	int method_number = parser.get<int>("method_num");
	if(method_number != 0)
	{
		method_num = method_number;
		std::cout<<"method chosen : ";

		if(method_number == 1)
		{
			std::cout<<method_number<<" : circles detection without image pretreatment"<<std::endl;
		}
		if(method_number == 2)
		{
			std::cout<<method_number<<" : circles detection with image pretreatment (Canny filtering, Smoothing, Dilatation, Erosion)"<<std::endl;
		}
	}
	else
	{
		std::cout<<"Circles detection without image pretreatment is used by default"<<std::endl;
	}


}
