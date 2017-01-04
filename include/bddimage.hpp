#ifndef BDDIMAGE_HPP
#define BDDIMAGE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <sstream>

#include "opencv2/opencv.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;

typedef std::map<int,std::vector<Mat> > MatMap;
typedef std::vector<KeyPoint> ImgKeyPoints;
typedef std::map<int, std::vector<ImgKeyPoints> > KpMap;

class BddImage{

	//The key are the value of the coin (in cent e.g : 1,5,10,20,50,100,200)
	MatMap imgMap; //original image
	MatMap greyImgMap; //img are in grey
	KpMap kpMap; //Map of keypoints
	MatMap descriptorMap; //map of the descriptor

	bool kpLoad;
	bool imgLoad;
	bool descriptorLoad;

	void loadImgFromFolder(std::string pathToImgFolder);

	public:
	BddImage(std::string pathToImgFolder = "./img/bdd/");
	//Later : think about a keypoint writer/reader 

	bool areKpLoad();
	bool areImgLoad();
	bool areDescriptorLoad();
	
	MatMap& getDescriptorMap();
	MatMap& getOriginalImgs();
	MatMap& getImgMap();
	KpMap& getKpMap();
	
	const MatMap& getconstOriginalImgs();
	const MatMap& getconstImgMap();
	const KpMap& getconstKpMap();
	const MatMap& getconstDescriptorMap();

	//void clean();
	//void reload();
};

#endif //BDDIMAGE_HPP
