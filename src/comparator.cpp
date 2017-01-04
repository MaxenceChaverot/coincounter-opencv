#include "comparator.hpp"

Comparator::Comparator(Mat img_in, std::string algorithmName):img_source(img_in)
{
	bdd = imread("img/bdd/2_cent.png");
	// Convert it to gray
	cvtColor(bdd, bdd_gray, CV_BGR2GRAY);
	//-------------------------------------------------------------------
	
	cvtColor(img_source, img_gray, CV_BGR2GRAY);

	//Orb descriptor constructor called by default
	feature_detector = FeatureDetector::create(algorithmName);
	descriptor_extractor = DescriptorExtractor::create(algorithmName);
	matcher = DescriptorMatcher::create("BruteForce-Hamming");
}

void Comparator::findKeyPointAndDescriptor(){

	if(img_gray.cols < 0 || img_gray.rows < 0){
		std::cout<<"Problem Image size < 0"<<std::endl;
		exit(0);
	}

	//First Step : Find Interest point

	//_________________________________________________//
	feature_detector->detect(bdd_gray,keypoints_bdd);
	//_________________________________________________//
	
   /* if(!imgBdd.areKpLoad()) imgBdd.computeKeypoint(feature_detector);*/
	feature_detector->detect(img_gray,keypoints_to_match);	

	//Second Step : Find Descriptor
	
	//_________________________________________________//
	descriptor_extractor->compute(bdd_gray, keypoints_bdd,descriptors_bdd);
	//_________________________________________________//
	
/*    for(MatMap::const_iterator it = imgBdd.getImgMap().begin(); it != imgBdd.getImgMap().end(); ++it){*/
	
		
	/*}*/

	descriptor_extractor->compute(img_gray, keypoints_to_match,descriptors_to_match);


}

void Comparator::match(){

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
}
