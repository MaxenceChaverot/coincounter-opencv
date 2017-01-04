#include "bddimage.hpp"

BddImage::BddImage(std::string pathToImgFolder):kpLoad(false),imgLoad(false),descriptorLoad(false)
{

	pathToBddFolder = cleanPathString(pathToImgFolder);
	loadImg();	
	std::cout<<imgMap.size()<<std::endl;

}

std::string BddImage::cleanPathString(std::string path){

	std::ostringstream s;
	s << path;
	//If the path string doesn't end by '/' 
	if(path[path.size()-1] != '/') s <<"/";
	s <<"*_cent.png";
	std::string str (s.str());
	return str;
}


void BddImage::loadImg(){

	std::vector<std::string> fn;

	//cv::glob method to find all the image
	glob(pathToBddFolder, fn, true);

	//Retrieve the label and put img in Map
	for(std::size_t i = 0; i < fn.size(); ++i){
		int pos = 0;
		int pos_end = fn[i].find("_cent.png");
		std::ostringstream s;

		//std::cout<<fn[i]<<" : "<<pos_end<<std::endl;
		if(pos_end != std::string::npos){

			for(int k = pos_end-1; k > 0; --k){
				if (fn[i][k] == '/'){
					pos=k+1;
					k = -1;
				}
			}


			for(int l = pos; l < pos_end; ++l){
				s << fn[i][l];	
			}

			std::string label(s.str());
			std::istringstream is(label);
			int key;
			is >> key;

			Mat img = imread(fn[i]);
			imgMap[key].push_back(img);
			Mat img_gray;
			cvtColor(img, img_gray, CV_BGR2GRAY);
			greyImgMap[key].push_back(img_gray);

			//std::cout<<"Label : "<< label <<std::endl;
			//std::cout<<"Label : "<< key  <<std::endl;

		}

	}

	imgLoad = true;
}


bool BddImage::areImgLoad(){ return imgLoad;}
bool& BddImage::getKpLoad(){ return kpLoad;}
bool& BddImage::getDescriptorLoad(){ return descriptorLoad;}

MatMap& BddImage::getOriginalImgs(){ return imgMap;}
MatMap& BddImage::getImgMap(){ return greyImgMap;}
KpMap& BddImage::getKpMap(){ return kpMap;}
MatMap& BddImage::getDescriptorMap(){ return descriptorMap;}

const MatMap& BddImage::getconstOriginalImgs(){ return imgMap;}
const MatMap& BddImage::getconstImgMap(){ return greyImgMap;}
const KpMap& BddImage::getconstKpMap(){ return kpMap;}
const MatMap& BddImage::getconstDescriptorMap(){ return descriptorMap;}

void BddImage::clean(){

	imgMap.clear();
	greyImgMap.clear();
	kpMap.clear();
	descriptorMap.clear();

	kpLoad = false;
	imgLoad = false;
	descriptorLoad = false;
}

void BddImage::setBddFolder(std::string pathToImgFolder){
	pathToBddFolder = cleanPathString(pathToImgFolder);
} 

void BddImage::reload(){
	clean();
	loadImg();
}
