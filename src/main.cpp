#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <stdio.h>

#include "comparator.hpp"
#include "bddimage.hpp"
#include "circlesdetection.hpp"

using namespace cv;

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

int main(int argc, char* argv[])
{
	help();
	
    /*std::cout<<"Test Bdd"<<std::endl;

	BddImage imgbdd;

    std::cout<<"Done"<<std::endl;*/

	std::cout<<"Program start."<<std::endl; 
    
	Mat src;
    int detection_number_method = 1; // Circles detection without pretreatment is used by default

	/* Parsing Command Line Argument */

    // path = the path of the source image
    // method_num = if 0, the detection is done without pretreatment, if 1, the detection is done with pretreatment
    const char* keys = "{path ||}{path_bbd ||}{method_num ||}{ip_num ||}";

    CommandLineParser parser(argc, argv, keys);

    // Path of the source image
    String im_path = parser.get<String>("path");
    if(im_path.empty())
    {
        src = imread("img/piecestest1.jpg", 1);
    }
    else
    {
        std::cout<<"path argument : "<<im_path<<std::endl;
        src = imread(im_path);
    }

    // Detection method (with or without pretreatment)
    int method_number = parser.get<int>("method_num");
    if(method_number != 0)
    {
        detection_number_method = method_number;
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

    /* Circles Detection */   
    CirclesDetection circles_Detection(src);
    circles_Detection.DetectCircles(detection_number_method);
    std::vector<Mat> extractedCircles = circles_Detection.GetOutputExtractedCircles();
    Mat img_with_circles_drawn = circles_Detection.GetOutputImage();

    std::cout<<"Number of coins finded : "<<extractedCircles.size()<<std::endl;

    /* Show original Image with circles detected drawn */

	namedWindow("Original image with coins", WINDOW_NORMAL);
    resizeWindow("Original image with coins", img_with_circles_drawn.cols, img_with_circles_drawn.rows);
    imshow("Original image with coins", img_with_circles_drawn);

     /* Comparison with the database */

    /*
	Comparator orb_comparator(extractedCircles[0]);
	orb_comparator.findKeyPointAndDescriptor();
	orb_comparator.match();

	Mat output = orb_comparator.outputMatches();

	// Show results for DEBUG
    for(size_t i = 0; i < extractedCircles.size(); i++)
    {
	   std::ostringstream s;
	   s << "Extracted Circle "<< i+1;
	   std::string nameWin(s.str());
	   namedWindow(nameWin, WINDOW_NORMAL);
	   resizeWindow(nameWin, extractedCircles[i].cols, extractedCircles[i].rows);
	   imshow(nameWin, extractedCircles[i]);
     }

	namedWindow("Matches", WINDOW_NORMAL);
	resizeWindow("Matches", output.cols, output.rows);
    imshow("Matches", output);*/

	waitKey(0);

	return 0;
}

