#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

typedef struct harrisCornerParameter{
	int blockSize;
	int apertureSize;
	double k;
} harrisCornerParameter;

class Comparator{

	Mat imgToCompare;
	harrisCornerParameter cornerParameter;

	public:
	Comparator();
};
