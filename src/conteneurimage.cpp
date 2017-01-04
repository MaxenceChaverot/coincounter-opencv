#include "conteneurimage.hpp"

ConteneurImage::ConteneurImage():label(-1){}

ConteneurImage::ConteneurImage(std::string pathToImg, bool labelInFileName){

	img = imread(pathToImg);
	cvtColor(img, img_grey, CV_BGR2GRAY);

	//Retrieve Label
	if(labelInFileName) retrieveLabelInFileName(pathToImg);
}


ConteneurImage::ConteneurImage(Mat M, int _label):img(M),label(_label){}

ConteneurImage::ConteneurImage(std::vector<KeyPoint> kp, int _label):keypoints(kp),label(_label){}

ConteneurImage::ConteneurImage(std::vector<KeyPoint> kp, Mat _descriptors, int _label):keypoints(kp),descriptors(_descriptors),label(_label){}


bool ConteneurImage::isImgLoad(){
	return img.empty();
}
bool ConteneurImage::areKpLoad(){
	return keypoints.empty();
}
bool ConteneurImage::areDescriptorsLoad(){
	return descriptors.empty();
}

void ConteneurImage::retrieveLabelInFileName(std::string pathToImg){

	int pos = 0;
	int pos_end = pathToImg.find("_cent.png");

	std::ostringstream s;

	if(pos_end != std::string::npos){

		for(int k = pos_end-1; k > 0; --k){
			if (pathToImg[k] == '/'){
				pos=k+1;
				k = -1;
			}
		}


		for(int l = pos; l < pos_end; ++l){
			s << pathToImg[l];	
		}

		std::string label(s.str());
		std::istringstream is(label);
		is >> label;
	}
}

void ConteneurImage::loadImgFromDisk(std::string pathToImg){

	clear();

	img = imread(pathToImg);
	cvtColor(img, img_grey, CV_BGR2GRAY);
}

void ConteneurImage::clear(){

	Mat empty;

	if(isImgLoad()){
		img = empty;
		img_grey = empty;
	}

	if(areKpLoad())	keypoints.clear();
	if(areDescriptorsLoad()) descriptors = empty;

	label = -1;
}
