#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;


class Comparator{

	Mat bdd, bdd_gray, img_source, img_gray;

	Ptr<FeatureDetector> orb_feature_detector;
	Ptr<DescriptorExtractor> descriptor_extractor;
	Ptr<DescriptorMatcher> matcher;

	std::vector<KeyPoint> keypoints_bdd, keypoints_to_match;
	Mat descriptors_bdd, descriptors_to_match;
	std::vector<DMatch> matches;

	public:
	Comparator(Mat img_in);
	void setBddImg(std::string img_path);
	void findKeyPointAndDescriptor();
	void match();
	Mat outputMatches();
};
