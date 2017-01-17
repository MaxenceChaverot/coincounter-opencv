#include "comparator_template_matching.hpp"

// Ctor
Comparator_TM::Comparator_TM()
{

}

// Perform the template matching operation
double Comparator_TM::template_match(Mat source_img, Mat templ, Mat homography)
{
    Mat result, img_h;

    // Apply Homography
    warpPerspective(source_img, img_h, homography, source_img.size());

    // Matching methods:
    // 0: SQDIFF, 1: SQDIFF NORMED, 2: TM CCORR, 3: TM CCORR NORMED
    // 4: TM COEFF, 5: TM COEFF NORMED"
    int match_method  = 0;

    if(img_h.rows >= templ.rows && img_h.cols >= templ.cols) matchTemplate(img_h, templ, result, match_method);
    else if(img_h.rows <= templ.rows && img_h.cols <= templ.cols) matchTemplate(templ, img_h, result, match_method);

    // Normalize the results
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    // Localize the minimum and maximum values in the result matrix
    double minVal, maxVal;
    Point minLoc, maxLoc;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    // For the first two methods, the best match are the lowest values
    // For all the others, higher values represent better matches
    double score;
    if(match_method  == 0 || match_method == 1) score = minVal;
    else score = maxVal;

    return score;
}
