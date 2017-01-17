#include "comparator_template_matching.hpp"

// Ctor
Comparator_TM::Comparator_TM()
{

}

// Perform the template matching operation
Mat Comparator_TM::template_match(Mat img, Mat templ)
{
    Mat result;

    int result_cols =  img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create(result_rows, result_cols, CV_32FC1);

    // Matching methods:
    // 0: SQDIFF, 1: SQDIFF NORMED, 2: TM CCORR, 3: TM CCORR NORMED
    // 4: TM COEFF, 5: TM COEFF NORMED"
    int match_method  = 2;
    matchTemplate(img, templ, result, 2);

    // Normalize the results
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    // Localize the minimum and maximum values in the result matrix
    double minVal, maxVal;
    Point minLoc, maxLoc;
    Point matchLoc;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    // For the first two methods, the best match are the lowest values
    // For all the others, higher values represent better matches
    // So, we save the corresponding value in the matchLoc variable:
    if(match_method  == 0 || match_method == 1) matchLoc = minLoc;
    else matchLoc = maxLoc;

    // Display the source image and the result matrix
    // Draw a rectangle around the highest possible matching area:
    Mat im_result = img;
    rectangle(im_result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

    return im_result;
}
