#include "comparator.hpp"

// Ctor
Comparator::Comparator(std::string algorithmName)
{
    // Orb descriptor constructor called by default
	feature_detector = FeatureDetector::create(algorithmName);
	descriptor_extractor = DescriptorExtractor::create(algorithmName);
	matcher = DescriptorMatcher::create("BruteForce-Hamming");
}


std::vector<KeyPoint> Comparator::findKeyPoints(Mat img)
{
    if(img.cols < 0 || img.rows < 0)
    {
        std::cout<<"Problem Image size < 0"<<std::endl;
        exit(0);
    }

    // Convert image source to gray
    Mat img_gray;
    cvtColor(img, img_gray, CV_BGR2GRAY);

    // Find interest points
    std::vector<KeyPoint> keypoints;
    feature_detector->detect(img_gray,keypoints);

    return keypoints;
}


Mat Comparator::findDescriptors(Mat img, std::vector<KeyPoint> keypoints)
{
    Mat descriptors;
    descriptor_extractor->compute(img, keypoints, descriptors);

    return descriptors;
}

/*
void Comparator::match()
{

	int min_dist = 100;
	int max_dist = 0;

	//Third Step : Match It
	Mat mask;
	matcher->match(descriptors_to_match,descriptors_bdd, matches);	

	//Find good matches = distance lower than 3*min_dist
	
	for( int i = 0; i < descriptors_to_match.rows; i++ )
	{
	    int dist = matches[i].distance;
	
	    if( dist < min_dist ) 
	        min_dist = dist;
	    if( dist > max_dist ) 
	        max_dist = dist;
	}

	for( int i = 0; i < descriptors_to_match.rows; i++ )
	{ 
	    if( matches[i].distance <= std::max((float)3*min_dist, (float)0.02) )
	        good_matches.push_back( matches[i]); 
	}

	std::cout<<"Good Match : "<<good_matches.size()<<std::endl;
	

	

	//Mat H = findHomography(img_gray,bdd_gray,CV_RANSAC,mask)
}

Mat Comparator::outputMatches(){

	Mat output;

	drawMatches(img_source,keypoints_to_match,bdd,keypoints_bdd,matches,output);

	return output;
}


void Comparator::setBddImg(std::string img_path){

	bdd = imread(img_path);
	cvtColor(bdd, bdd_gray, CV_BGR2GRAY);
}*/
