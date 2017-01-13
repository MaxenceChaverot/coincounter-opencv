#include "bddimage.hpp"

BddImage::BddImage(std::string pathToImgFolder)
{

	pathToBddFolder = cleanPathString(pathToImgFolder);
	loadImg();	
}

std::string BddImage::cleanPathString(std::string path){

	std::ostringstream s;
	s << path;

	//If the path string doesn't end by '/' -> add '/' to the string 
	if(path[path.size()-1] != '/') s <<"/";
	
	//Add regex for glob function
	s <<"*_cent*.png";
	std::string str (s.str());
	return str;
}


void BddImage::loadImg(){

	std::vector<std::string> fn;

	//cv::glob method to find all the image
	glob(pathToBddFolder, fn, true);
	
	std::cout<<"Bdd will be filed with "<<fn.size()<<" pictures"<<std::endl;

	for(int i = 0; i < fn.size(); ++i){
		ConteneurImage contImg(fn[i]);
		int label = retrieveLabelInFileName(fn[i]);
		if(label != -1)	bddMap[label].push_back(contImg);
	}
}

void BddImage::fillKpAndDescriptor(Comparator comparator){

	for(std::map<int, std::vector<ConteneurImage> >::iterator it = bddMap.begin(); it != bddMap.end(); ++it){
	
		int label = it->first;
		std::vector<ConteneurImage>& vector = it->second;
	
		for(int i = 0; i < it->second.size(); ++i){
	
			ConteneurImage& current = vector[i];
			current.keypoints.clear();
			current.keypoints = comparator.findKeyPoints(current.img);
			current.descriptors = comparator.findDescriptors(current.img, current.keypoints);
		}
	}
}

int BddImage::retrieveLabelInFileName(std::string pathToImg){

	int pos = 0;
	int pos_end = pathToImg.find("_cent");

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
		int label_int;
		is >> label_int;
		return label_int;
	}

	return -1;
}

std::map<int,std::vector<ConteneurImage> >& BddImage::getMap(){ return bddMap;}
