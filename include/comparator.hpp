#ifndef COMPARATOR_HPP
#define COMPARATOR_HPP

#include <iostream>
#include <algorithm>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "bddimage.hpp"

using namespace cv;

class Comparator
{

private:

        /* ATTRIBUTES */

        // The homography
	//Mat homography;

        // Orb descriptor
	Ptr<FeatureDetector> feature_detector;
	Ptr<DescriptorExtractor> descriptor_extractor;
	Ptr<DescriptorMatcher> matcher;

        // Matches
	//std::vector<DMatch> matches, good_matches;
	//std::vector<Point2f> obj, scene;

public:

        // Ctor (algorithmName = ORB or MSER)
	Comparator(std::string algorithmName = "ORB");

        /* METHODS */

        // Find the interest points
        std::vector<KeyPoint> findKeyPoints(Mat img); 
        // Find the descriptors
        Mat findDescriptors(Mat img, std::vector<KeyPoint> keypoints);

	//void match();

	//Mat outputMatches();
};

#endif //COMPARATOR_HPP
