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

double Comparator::computeDistance(Point2f p1, Point2f p2, int size_x, int size_y){
	return std::sqrt((p2.x - p1.x)*(p2.x - p1.x)/(size_x*size_x)+(p2.y - p1.y)*(p2.y - p1.y)/(size_y*size_y));
}

std::vector<DMatch> Comparator::match(Mat descriptors_to_match, Mat descriptors_bdd)
{
	int min_dist = 500;
	int max_dist = 0;

	// Match the two descriptors
	std::vector<DMatch> matches;
	matcher->match(descriptors_to_match,descriptors_bdd, matches);	

	// Eliminate Matches with distance lower than 3*min_dist
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

	return good_matches;
}

Mat Comparator::drawOutputMatches(Mat img_source, Mat img_bdd, std::vector<KeyPoint> keypoints_to_match, std::vector<KeyPoint> keypoints_bdd, std::vector<DMatch> matches)
{
	Mat output;
	drawMatches(img_source, keypoints_to_match, img_bdd, keypoints_bdd, matches, output);

	return output;
}

Mat Comparator::GetHomography(std::vector<KeyPoint> kp1,std::vector<KeyPoint> kp2,std::vector<DMatch> matches, OutputArray mask)
{
	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for( int i = 0; i < matches.size(); i++ )
	{
		//-- Get the keypoints from the good matches
		obj.push_back( kp1[matches[i].queryIdx].pt );
		scene.push_back( kp2[matches[i].trainIdx].pt );
	}

	return findHomography( obj, scene, CV_RANSAC,3, mask); 
}

double Comparator::retrieveNbInliers(Mat mask){

	//Mauvais Score : preférer le nombre d'inlier
	double sum = 0.0;
	for(int i = 0; i < mask.rows; ++i){
		sum += mask.at<uchar>(i,0);
	}

	return sum;
}


double Comparator::normalizeCC(Mat source_img, Mat bdd_img, Mat homography){

	Mat result, img_h;

	//Apply Homography
	warpPerspective(source_img, img_h, homography, source_img.size());

	//Normalized Cross Corelation	
	if(img_h.rows >= bdd_img.rows && img_h.cols >= bdd_img.cols) matchTemplate(img_h,bdd_img,result,CV_TM_CCOEFF_NORMED);
	else if(img_h.rows <= bdd_img.rows && img_h.cols <= bdd_img.cols) matchTemplate(bdd_img,img_h,result,CV_TM_CCOEFF_NORMED);

	//Find a best match:
	double minVal, maxVal;
	Point minLoc, maxLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

	return maxVal;
}

double Comparator::meanInliersDistance(Mat source_img,std::vector<KeyPoint> keypoints , Mat mask){

	std::vector<Point2f> inliers;
	int size_x = source_img.cols;
	int size_y = source_img.rows;

	//Retrieve Good Keypoints after RANSAC Homography
	for(int i = 0; i < keypoints.size() ; ++i){
		if(mask.at<uchar>(i,0) == 1){
			inliers.push_back(keypoints[i].pt);
		}
	}


	//Calcul des distances minimales et maximales entre les Inliers
   /* double min_dist = 500;*/
	/*double max_dist = 0;*/

   /* for(int i = 0; i < inliers.size(); i++){*/
		//for(int j = 0; j < inliers.size(); j++){
			//double dist = computeDistance(inliers.at(i),inliers.at(j),size_x,size_y);	
			//if( dist < min_dist && dist > 0.0f) 
				//min_dist = dist;
			//if( dist > max_dist ) 
				//max_dist = dist;
		//}
	//}


	//On fait la moyenne des distances entre chaque Inliers
	double sum = 0.0f;
	int total=0;

	for(int i = 0; i < inliers.size(); i++){
		for(int j = 0; j < inliers.size(); j++){
			double distance;
			distance = computeDistance(inliers.at(i),inliers.at(j),size_x,size_y);
			//On ne considère pas les Inliers trop proche entre eux
			if(distance > 0.1f){
				sum += distance;
				total++;
			}
		}
	}

	double result;
	result = sum/total;
	return result;
}
