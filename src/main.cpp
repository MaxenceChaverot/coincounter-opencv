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

typedef enum SCORE_METHOD{ NB_INLIERS, ZERO_MEAN_CC, KEYPOINTS_MEAN_DISTANCE} SCORE_METHOD;

int main(int argc, char* argv[])
{
	// Help message
	help();

	//Command Line Parameters Defaults
	std::string im_path = "img/piecestest1.jpg",bdd_path="img/bdd/";
	std::string ip_num="ORB"; //ORB Detector used by default
	int method_num=1; // Circles detection without pretreatment is used by default
	SCORE_METHOD score_method = KEYPOINTS_MEAN_DISTANCE;

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
	//resizeWindow("Original image with coins", img_with_circles_drawn.cols, img_with_circles_drawn.rows);
	resizeWindow("Original image with coins", 500,500);
	imshow("Original image with coins", img_with_circles_drawn);

	std::cout<<"Press Enter to continue !"<<std::endl<<std::endl;
	waitKey(0);

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

	for(int i = 0; i < circleToCompare.size();++i){

		std::cout<<"Computing..."<<std::endl;

		ConteneurImage circle = circleToCompare[i];	

		std::map<int,std::vector<ConteneurImage> >::iterator it,itBest;

		//int bestScore = 0;
		double bestScore = 0;
		int bestIndex = -1;
		std::vector<DMatch> bestMatches;

		//Donner les X meilleurs dans l'ordre avec les scores
		//Donner premier, si KO, donnez le suivant
		//Trier base de donnée avec les scores
		for(it = bdd.getMap().begin(); it != bdd.getMap().end(); ++it){
			for(int i = 0; i < it->second.size(); ++i){

				std::vector<DMatch> matches = comparator.match(circle.descriptors,it->second[i].descriptors);

				Mat homography,mask;
				homography = comparator.GetHomography(circle.keypoints,it->second[i].keypoints, matches, mask);	

				double score;

				switch(score_method){

					case NB_INLIERS:
						score = comparator.retrieveNbInliers(mask); 
						break;
					case ZERO_MEAN_CC:
						score = comparator.normalizeCC(circle.img,it->second[i].img,homography);
						break;
					case KEYPOINTS_MEAN_DISTANCE:
						score = comparator.meanInliersDistance(circle.img, circle.keypoints, mask);
						break;
					default:
						exit(0);
				}
				//DEBUG
				//std::cout<<"For label = "<<it->first<<", Score = "<<score<<std::endl;

				if(score > bestScore){
					bestScore = score;
					itBest = it;
					bestIndex = i;
					bestMatches = matches;
				}
			}
		}

		std::cout<<"The coin Value is "<<itBest->first<<std::endl<<std::endl;

		//Draw the best Match
		Mat output = comparator.drawOutputMatches(circle.img,itBest->second[bestIndex].img,circle.keypoints,itBest->second[bestIndex].keypoints,bestMatches);

		namedWindow("Coin", WINDOW_NORMAL);
		resizeWindow("Coin", circle.img.cols, circle.img.rows);
		imshow("Coin",circle.img);

		namedWindow("Matches", WINDOW_NORMAL);
		resizeWindow("Matches", output.cols, output.rows);
		imshow("Matches", output);
		std::cout<<"Press Enter to continue !"<<std::endl<<std::endl;
		waitKey(0);
	}
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

	std::cout<<std::endl;
}
