#ifndef COMPARATOR_TM_HPP
#define COMPARATOR_TM_HPP

#include <iostream>
#include <algorithm>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace cv;

class Comparator_TM
{

private:

        /* ATTRIBUTES */


public:

        /* METHODS */

        // Ctor
        Comparator_TM();

        // Perform the template matching operation
        double template_match(Mat source_img, Mat templ, Mat homography);
};

#endif //COMPARATOR_TM_HPP
