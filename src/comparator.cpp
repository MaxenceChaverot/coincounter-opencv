#include "comparator.hpp"

Comparator::Comparator(Mat img_in):img_source(img_in)
{
	bdd = imread("img/bdd/1cent.png");
	// Convert it to gray
	cvtColor(bdd, bdd_gray, CV_BGR2GRAY);
	cvtColor(img_source, img_gray, CV_BGR2GRAY);

	orb_feature_detector = FeatureDetector::create("ORB");

	//Orb descriptor constructor called by default
	descriptor_extractor = DescriptorExtractor::create("ORB");
	matcher = DescriptorMatcher::create("BruteForce-Hamming");
}

void Comparator::findKeyPointAndDescriptor(){

	if(bdd_gray.cols < 0 || bdd_gray.rows < 0){
		std::cout<<"Problem Image size < 0"<<std::endl;
		exit(0);
	}

	//First Step : Find Interest point

	orb_feature_detector->detect(bdd_gray,keypoints_bdd);
	orb_feature_detector->detect(img_gray,keypoints_to_match);	
	//std::cout<<"Nb features "<<keypoints_to_match.size()<<std::endl;

	//Second Step : Find Descriptor

	descriptor_extractor->compute(bdd_gray, keypoints_bdd,descriptors_bdd);
	descriptor_extractor->compute(img_gray, keypoints_to_match,descriptors_to_match);


}

void Comparator::match(){

	//Third Step : Match It

	matcher->match(descriptors_to_match,descriptors_bdd, matches);	
	//std::cout<<"Nb descri = "<<matches.size()<<std::endl;
}

Mat Comparator::outputMatches(){

	Mat output;

	drawMatches(img_source,keypoints_to_match,bdd,keypoints_bdd,matches,output);

	return output;
}


void Comparator::setBddImg(std::string img_path){

	bdd = imread(img_path);
	cvtColor(bdd, bdd_gray, CV_BGR2GRAY);
}
