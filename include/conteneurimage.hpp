#ifndef CONTENEURIMAGE_HPP
#define CONTENEURIMAGE_HPP

#include <iostream>
#include <vector>

#include "opencv2/opencv.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;


class ConteneurImage{
	
	public:

		Mat img;
		
		std::vector<KeyPoint> keypoints;
		Mat descriptors;

		ConteneurImage();
		~ConteneurImage();
		ConteneurImage(std::string pathToImg);
		ConteneurImage(Mat M);
		ConteneurImage(std::vector<KeyPoint> kp);
		ConteneurImage(std::vector<KeyPoint> kp, Mat _descriptors);

		bool isImgLoad();
		bool areKpLoad();
		bool areDescriptorsLoad();

		void clear();

};



#endif //CONTENEURIMAGE_HPP
