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
		Mat img_grey;
		
		std::vector<KeyPoint> keypoints;
		Mat descriptors;

		//If label = -1, image is unlabelled
		int label;

		ConteneurImage();
		ConteneurImage(std::string pathToImg, bool labelInFileName = false);
		ConteneurImage(Mat M, int _label = -1);
		ConteneurImage(std::vector<KeyPoint> kp, int _label = -1);
		ConteneurImage(std::vector<KeyPoint> kp, Mat _descriptors, int label = -1);

		bool isImgLoad();
		bool areKpLoad();
		bool areDescriptorsLoad();

		void loadImgFromDisk(std::string pathToImg);
		void retrieveLabelInFileName(std::string pathToImg);

		void clear();

};



#endif //CONTENEURIMAGE_HPP
