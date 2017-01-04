#ifndef COMPARATOR_HPP
#define COMPARATOR_HPP

#include <iostream>
#include <algorithm>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "bddimage.hpp"

using namespace cv;

class Comparator{

	Mat bdd, bdd_gray, img_source, img_gray, homography;

	BddImage imgBdd;

	Ptr<FeatureDetector> feature_detector;
	Ptr<DescriptorExtractor> descriptor_extractor;
	Ptr<DescriptorMatcher> matcher;

	//std::vector< std::vector<KeyPoint> > keypoints_bdd;
	std::vector<KeyPoint> keypoints_bdd, keypoints_to_match;
	Mat descriptors_bdd, descriptors_to_match;
	std::vector<DMatch> matches, good_matches;
	std::vector<Point2f> obj, scene;

	public:
	Comparator(Mat img_in, std::string algorithmName = "ORB");
	void setBddImg(std::string img_path);
	void findKeyPointAndDescriptor();
	void match();
	Mat outputMatches();
};

#endif //COMPARATOR_HPP
