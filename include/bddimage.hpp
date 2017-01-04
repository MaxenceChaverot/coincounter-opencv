#ifndef BDDIMAGE_HPP
#define BDDIMAGE_HPP

#include <iostream>
#include <vector>
#include <map>
#include <sstream>

#include "opencv2/opencv.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "comparator.hpp"
#include "conteneurimage.hpp"

using namespace cv;

class BddImage{

	//The key are the value of the coin (in cent e.g : 1,5,10,20,50,100,200)
	std::map<int,std::vector<ConteneurImage> > bddMap;	

	std::string pathToBddFolder;

	int size;

	std::string cleanPathString(std::string path);

	void loadImg();

	public:

	BddImage(std::string pathToImgFolder = "./img/bdd/");
	//Later : think about a keypoint writer/reader 

	void fillKpAndDescriptor(Comparator comparator);
	int retrieveLabelInFileName(std::string pathToImg);

	std::map<int,std::vector<ConteneurImage> >& getMap();
};

#endif //BDDIMAGE_HPP
