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
    // Check input img
    if(img.cols < 0 || img.rows < 0)
    {
        std::cout<<"Problem Image size < 0"<<std::endl;
        exit(0);
    }

    // Check input keypoints
    if(keypoints.size() == 0)
    {
        std::cout<<"Problem keypoints size = 0"<<std::endl;
        exit(0);
    }

    Mat descriptors;
    descriptor_extractor->compute(img, keypoints, descriptors);

    return descriptors;
}


std::vector<DMatch> Comparator::match(Mat descriptors_to_match, Mat descriptors_bdd)
{
	int min_dist = 100;
	int max_dist = 0;

    // Match the two descriptors
    std::vector<DMatch> matches;
	matcher->match(descriptors_to_match,descriptors_bdd, matches);	

    // Find good matches = distance lower than 3*min_dist
    std::vector<DMatch> good_matches;
    for(int i = 0; i < descriptors_to_match.rows; i++)
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
            good_matches.push_back(matches[i]);
	}

    std::cout<<"Good Matches : "<<good_matches.size()<<std::endl;
	
    return good_matches;
}

Mat Comparator::drawOutputMatches(Mat img_source, Mat img_bdd, std::vector<KeyPoint> keypoints_to_match, std::vector<KeyPoint> keypoints_bdd, std::vector<DMatch> matches)
{
	Mat output;
    drawMatches(img_source, keypoints_to_match, img_bdd, keypoints_bdd, matches, output);

	return output;
}

Mat Comparator::GetHomography(Mat img_source, Mat img_bdd)
{
    Mat mask;

    // Convert image source to gray
    Mat img_gray;
    cvtColor(img_source, img_gray, CV_BGR2GRAY);

    // Convert image source to gray
    Mat img_bdd_gray;
    cvtColor(img_bdd, img_bdd_gray, CV_BGR2GRAY);

    Mat H = findHomography(img_gray, img_bdd_gray, mask, CV_RANSAC);

    return H;
}
