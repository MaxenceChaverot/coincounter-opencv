#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;

class CirclesDetection
{

private:
	Mat img_source, img_gray;
        int detection_methode_number;
	std::vector<Mat> extractedCircles;

public:
        // Ctor
	CirclesDetection(Mat img_in, int detection_methode_number);

        // Pre traitment functions
	Mat CannyFilter();
        Mat Smooth(Mat img);
        Mat Dilatation(Mat img);
        Mat Erosion(Mat img);

        void ConvertToGray(Mat img);

        // Draw circles on the image source
        void DrawCirclesOnImageSource(std::vector<Vec3f> circles);
        // get extracted circles
        void FillExtractedCircles(std::vector<Vec3f> circles);

        // First detection method (detection_methode_number = 0)
        void CirclesDetectionWithPreTraitment();
        // Second detection method (detection_methode_number = 1)
        void CirclesDetectionWithoutPreTraitment():

        void DetectCircles();
	std::vector<Mat> GetOutputExtractedCircles();
        std::vector<Mat> GetOutputImage();
};
