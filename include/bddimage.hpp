#ifndef BDDIMAGE_HPP
#define BDDIMAGE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <sstream>

#include "opencv2/opencv.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "comparator.hpp"

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

	std::string pathToBddFolder;

	bool imgLoad;
	bool kpLoad;
	bool descriptorLoad;

	std::string cleanPathString(std::string path);
	void loadImg();

	public:
	BddImage(std::string pathToImgFolder = "./img/bdd/");
	//Later : think about a keypoint writer/reader 

	bool areImgLoad();
	bool& getKpLoad();
	bool& getDescriptorLoad();
	
	MatMap& getDescriptorMap();
	MatMap& getOriginalImgs();
	MatMap& getImgMap();
	KpMap& getKpMap();
	
	const MatMap& getconstOriginalImgs();
	const MatMap& getconstImgMap();
	const KpMap& getconstKpMap();
	const MatMap& getconstDescriptorMap();

	void clean();
	void reload();
	void setBddFolder(std::string pathToImgFolder);
	void fillKpMap(Comparator comparator);
	void fillDescriptorMap(Comparator comparator);
};

#endif //BDDIMAGE_HPP
