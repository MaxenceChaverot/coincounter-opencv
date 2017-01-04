#ifndef COMPARATOR_HPP
#define COMPARATOR_HPP

#include <iostream>
#include <algorithm>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;

class Comparator
{

private:

        /* ATTRIBUTES */

        // Orb descriptor
	Ptr<FeatureDetector> feature_detector;
	Ptr<DescriptorExtractor> descriptor_extractor;
	Ptr<DescriptorMatcher> matcher;

public:

        /* METHODS */

        // Ctor (algorithmName = ORB or MSER)
	Comparator(std::string algorithmName = "ORB");

        // Find the interest points
        std::vector<KeyPoint> findKeyPoints(Mat img);

        // Find the descriptors
        Mat findDescriptors(Mat img, std::vector<KeyPoint> keypoints);

        // Find the good matches
        std::vector<DMatch> match(Mat descriptors_to_match, Mat descriptors_bdd);

        // Draw the matches
	Mat drawOutputMatches(Mat img_source, Mat img_bdd, std::vector<KeyPoint> keypoints_to_match, std::vector<KeyPoint> keypoints_bdd, std::vector<DMatch> matches);

        // Get the homography between two images
        Mat GetHomography(std::vector<KeyPoint> kp1,std::vector<KeyPoint> kp2,std::vector<DMatch> matches, int& outputScore);

		//Calculate the score 
		int calculateScore(Mat mask);
};

#endif //COMPARATOR_HPP
