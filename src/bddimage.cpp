#include "bddimage.hpp"

BddImage::BddImage(std::string pathToImgFolder):kpLoad(false),imgLoad(false)
{

	loadImgFromFolder(pathToImgFolder);	
	//std::cout<<imgMap.size()<<std::endl;

}

void BddImage::loadImgFromFolder(std::string pathToImgFolder){


	std::ostringstream s;
	s << pathToImgFolder;

	//If the path string doesn't end by '/' 
	if(pathToImgFolder[pathToImgFolder.size()-1] != '/') s <<"/";

	s <<"*_cent.png";
	std::string path (s.str());
	std::vector<std::string> fn;

	//cv::glob method to find all the image
	glob(path, fn, true);

	//Retrieve the label and put img in Map
	for(std::size_t i = 0; i < fn.size(); ++i){
		int pos = 0;
		int pos_end = fn[i].find("_cent.png");
		std::ostringstream q;

		//std::cout<<fn[i]<<" : "<<pos_end<<std::endl;
		if(pos_end != std::string::npos){

			for(int k = pos_end-1; k > 0; --k){
				if (fn[i][k] == '/'){
					pos=k+1;
					k = -1;
				}
			}


			for(int l = pos; l < pos_end; ++l){
				q << fn[i][l];	
			}

			std::string label(q.str());
			std::istringstream is(label);
			int key;
			is >> key;

			Mat img = imread(fn[i]);
			imgMap.insert(std::pair<int,Mat>(key,img));
			Mat img_gray;
			cvtColor(img, img_gray, CV_BGR2GRAY);
			greyImgMap.insert(std::pair<int,Mat>(key,img_gray));

			//std::cout<<"Label : "<< label <<std::endl;
			//std::cout<<"Label : "<< key  <<std::endl;

		}

	}
}

void BddImage::computeKeypoint(Ptr<FeatureDetector> feature_detector){ return;}

bool BddImage::areKpLoad(){ return kpLoad;}
bool BddImage::areImgLoad(){ return imgLoad;}
const std::map<int,Mat>& BddImage::getOriginalImgs(){ return imgMap;}
const std::map<int, Mat>& BddImage::getImgMap(){ return greyImgMap;}
const std::map<int,KeyPoint>& BddImage::getKpMap(){ return kpMap;}
