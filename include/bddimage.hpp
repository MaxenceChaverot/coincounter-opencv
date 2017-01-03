#ifndef BDDIMAGE_HPP
#define BDDIMAGE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <sstream>

#include "opencv2/opencv.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;

class BddImage{

	//The key are the value of the coin (in cent e.g : 1,5,10,20,50,100,200)
	std::map<int,Mat> imgMap; //original image
	std::map<int,Mat> greyImgMap; //img are in grey
	std::map<int, KeyPoint> kpMap; //Map of keypoints

	bool kpLoad;
	bool imgLoad;

	void loadImgFromFolder(std::string pathToImgFolder);

	public:
	BddImage(std::string pathToImgFolder = "./img/bdd/");
	//Later : think about a keypoint writer/reader 

	bool areKpLoad();
	bool areImgLoad();
	const std::map<int, Mat>& getOriginalImgs();
	const std::map<int, Mat>& getImgMap();
	const std::map<int,KeyPoint>& getKpMap();

	void computeKeypoint(Ptr<FeatureDetector> feature_detector);
};

#endif //BDDIMAGE_HPP
