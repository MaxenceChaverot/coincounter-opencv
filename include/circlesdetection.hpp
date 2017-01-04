#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;

/* This class allows circles detection on a source image :
This detection can be done with or wihtout pretreatment on the image */

class CirclesDetection
{

private:

        /* ATTRIBUTES */

        // The source image and the gray pretreated image associated
	Mat img_source, img_gray;

        // The output vector containing all extracted circles detected by the detection method chosen
	std::vector<Mat> extractedCircles;

        /* METHODS */

        // Pretreatment functions
	Mat CannyFilter();
        Mat Smooth(Mat img);
        Mat Dilatation(Mat img);
        Mat Erosion(Mat img);

        void ConvertToGray(Mat img);

        // Draw circles on the image source
        void DrawCirclesOnImageSource(std::vector<Vec3f> circles);
        // Fill the vector of all extracted circles
        void FillExtractedCircles(std::vector<Vec3f> circles);

        // First detection method (detection_methode_number = 0)
        void CirclesDetectionWithPretreatment();
        // Second detection method (detection_methode_number = 1)
        void CirclesDetectionWithoutPretreatment();

public:
        // Ctor
        CirclesDetection(Mat img_in);

        // Circles detection function
        // The value of detection_methode_number can be 1 or 2 :
        // If 1 : the detection is done without any pretreatment
        // If 2 : the detection is done with pretreatment (Canny filter, dilatation, erosion)
        void DetectCircles(int detection_methode_number);

        // Get the vector containing all estracted circles
	std::vector<Mat> GetOutputExtractedCircles();
        // Get the source image with the detected circles drawn on it
        Mat GetOutputImage();
};
