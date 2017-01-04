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

	//Loading Image to Compare
	Mat src = imread(im_path);


	/****************************************************************/
	/* Circles Detection */   
	/****************************************************************/

	int detection_number_method = method_num; 
	CirclesDetection circles_Detection(src);
	circles_Detection.DetectCircles(detection_number_method);
	std::vector<Mat> extractedCircles = circles_Detection.GetOutputExtractedCircles();
	std::cout<<"Number of coins finded : "<<extractedCircles.size()<<std::endl;

	/****************************************************************/
	/* Create Vector of ConteneurImage with the extracted Circle*/   
	/****************************************************************/

	std::vector<ConteneurImage> circleToCompare;

	for(int i = 0; i < extractedCircles.size(); ++i){
		ConteneurImage circle(extractedCircles[i]);
		circleToCompare.push_back(circle);
	}

	/* Show original Image with circles detected drawn */
	Mat img_with_circles_drawn = circles_Detection.GetOutputImage();
	namedWindow("Original image with coins", WINDOW_NORMAL);
	resizeWindow("Original image with coins", img_with_circles_drawn.cols, img_with_circles_drawn.rows);
	imshow("Original image with coins", img_with_circles_drawn);

	/* Show results for DEBUG
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
	/* Create Comparator */
	/****************************************************************/

	Comparator comparator(ip_num); //ORB By Default

	/****************************************************************/
	/* Create and fill BDD */
	/****************************************************************/

	BddImage bdd(bdd_path);

	if(bdd.getMap().empty()){
	   	std::cout<<"Bdd loading error"<<std::endl;	
		exit(0);
	}


	bdd.fillKpAndDescriptor(comparator);
	
	std::cout<<"Bdd is filled with "<<bdd.getMap().size()<<" images."<<std::endl;

	/****************************************************************/
	/* Compute keypoints and descriptors of ConteneurImage inside the vector */
	/****************************************************************/
	
	for(int i = 0; i < circleToCompare.size(); ++i){
		circleToCompare[i].keypoints = comparator.findKeyPoints(circleToCompare[i].img);
		circleToCompare[i].descriptors = comparator.findDescriptors(circleToCompare[i].img, circleToCompare[i].keypoints);
	}
	
		
	/****************************************************************/
	/*(For only one circle extrated : TESTING)*/
	/****************************************************************/

	/****************************************************************/
	/*Compare Extracted Circle to Database*/
	/*  -For Each Image in the Database
	 *   	- Compute Matches between the images.
	 *  	- Compute Homography
	 *  	- Compute score
	 *  -Find the Best score */
	/****************************************************************/

	ConteneurImage circle = circleToCompare[3];	

	std::map<int,std::vector<ConteneurImage> >::iterator it,itBest;

	int bestScore = 0;
	int bestIndex = -1;
	std::vector<DMatch> bestMatches;

	for(it = bdd.getMap().begin(); it != bdd.getMap().end(); ++it){
		for(int i = 0; i < it->second.size(); ++i){
			int score = 0;	
			std::vector<DMatch> matches = comparator.match(circle.descriptors,it->second[i].descriptors);
		
			Mat H = comparator.GetHomography(circle.keypoints,it->second[i].keypoints, matches, score);	

			std::cout<<"For label = "<<it->first<<", Score = "<<score<<std::endl;

			if(score > bestScore){
				bestScore = score;
				itBest = it;
				bestIndex = i;
				bestMatches = matches;
			}
		}
	}

	std::cout<<"The coin Value is "<<itBest->first<<std::endl;

	//Draw the best Match
	Mat output = comparator.drawOutputMatches(circle.img,itBest->second[bestIndex].img,circle.keypoints,itBest->second[bestIndex].keypoints,bestMatches);

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
