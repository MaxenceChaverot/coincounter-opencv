#include "conteneurimage.hpp"

ConteneurImage::ConteneurImage(){}

ConteneurImage::~ConteneurImage(){ clear();}

ConteneurImage::ConteneurImage(std::string pathToImg){
	img = imread(pathToImg);
}


ConteneurImage::ConteneurImage(Mat M):img(M){}

ConteneurImage::ConteneurImage(std::vector<KeyPoint> kp):keypoints(kp){}

ConteneurImage::ConteneurImage(std::vector<KeyPoint> kp, Mat _descriptors):keypoints(kp),descriptors(_descriptors){}


bool ConteneurImage::isImgLoad(){
	return img.empty();
}
bool ConteneurImage::areKpLoad(){
	return keypoints.empty();
}
bool ConteneurImage::areDescriptorsLoad(){
	return descriptors.empty();
}

void ConteneurImage::clear(){

	Mat empty;

	if(isImgLoad()) img = empty;
	if(areKpLoad())	keypoints.clear();
	if(areDescriptorsLoad()) descriptors = empty;
}
