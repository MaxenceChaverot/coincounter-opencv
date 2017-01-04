#include "circlesdetection.hpp"

// Ctor
CirclesDetection::CirclesDetection(Mat img_in):img_source(img_in)
{}

// Image filtering (Canny Filter)
Mat CirclesDetection::CannyFilter()
{
    Mat filtered_image;
    Mat detected_edges;

    // Reduce noise with a kernel 3x3
    blur(img_source, detected_edges, Size(3,3));

    // Canny detector
    Canny(img_source, detected_edges, 80, 90);

    // Using Canny's output as a mask, we display our result
    filtered_image = Scalar::all(0);
    img_source.copyTo(filtered_image, detected_edges);

    // Return the filtered image
    return filtered_image;
}

// Image smoothing
Mat CirclesDetection::Smooth(Mat img)
{
    Mat smoothed_image;
    GaussianBlur(img, smoothed_image, cv::Size(9, 9), 2, 2);

    // Return smoothed image
    return smoothed_image;
}

// Image dilatation
Mat CirclesDetection::Dilatation(Mat img)
{
    Mat dilated_image;
    dilate(img, dilated_image, getStructuringElement(MORPH_ELLIPSE, cv::Size(16, 16)));

    // Return the dilated image
    return dilated_image;
}

// Image erosion
Mat CirclesDetection::Erosion(Mat img)
{
    Mat eroded_image;
    erode(img, eroded_image, getStructuringElement(MORPH_ELLIPSE, cv::Size(15, 15)));

    // Return the reoded image
    return eroded_image;
}

// Convert an image to gray
void CirclesDetection::ConvertToGray(Mat img)
{
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    GaussianBlur(img_gray, img_gray, cv::Size(9, 9),1,1);
}

// Draw the detected circles on the source image
void CirclesDetection::DrawCirclesOnImageSource(std::vector<Vec3f> circles)
{
    // Draw the circles detected on the original image
	for( size_t i = 0; i < circles.size(); i++ )
	{
        // DrawCircle
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);

        // Circle center
		circle(img_source, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // Circle outline
		circle(img_source, center, radius, Scalar(0,0,255), 3, 8, 0 );
	}
}

// Fill the vector of all extracted circles
void CirclesDetection::FillExtractedCircles(std::vector<Vec3f> circles)
{
    // Extract the circles detected
	for( size_t i = 0; i < circles.size(); i++ )
	{
        // Get the Rect containing the circle:
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
		Rect r(center.x-radius, center.y-radius, radius*2,radius*2);

        // Obtain the image ROI:
        Mat roi(img_source, r);

        // Make a black mask, same size:
		Mat mask(roi.size(), roi.type(), Scalar::all(0));
        // With a white, filled circle in it:
		circle(mask, Point(radius,radius), radius, Scalar::all(255), -1);

        // Combine roi & mask:
		Mat circleCropped = roi & mask;

        // Fill the output vector
		extractedCircles.push_back(circleCropped);
	}
}

// First detection method (detection_methode_number = 0) : without image pretreatment
void CirclesDetection::CirclesDetectionWithoutPretreatment()
{
    // Convert image to Gray
    cvtColor(img_source, img_gray, COLOR_BGR2GRAY);

    // Detect circles
    std::vector<Vec3f> circles;
    GaussianBlur(img_gray, img_gray, cv::Size(9, 9), 2, 2);
    HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT, 1, img_gray.rows/8, 100, 50, 0, 0);

    // Draw circles detected on image source
    DrawCirclesOnImageSource(circles);

    // Extract circles detected
    FillExtractedCircles(circles);
}

// Second detection method (detection_methode_number = 1) : with image pretreatment
void CirclesDetection::CirclesDetectionWithPretreatment()
{
    // Image pretreatment
    Mat filtered_image = CannyFilter();
    Mat smoothed_image = Smooth(filtered_image);
    Mat dilated_image = Dilatation(smoothed_image);
    Mat eroded_image = Erosion(dilated_image);
    ConvertToGray(eroded_image);

    // Apply the Hough Transform to find the circles
    // CV_HOUGH_GRADIENT : detection method to use (currently, the only implemented one)
    // 1 : inverse ratio of the accumulator resolution to the image resolution
    // src_gray.rows/8 : minimum distance between the centers of the detected circles
    // 200 : the higher threshold of the two passed to the Canny() edge detector (the lower one is twice smaller)
    // 100 : the accumulator threshold for the circle centers at the detection stage. The smaller it is, the more false circles may be detected
    // 0 : Minimum circle radius
    // 0 : Maximum circle radius

    std::vector<Vec3f> circles;
    HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT, 1, img_gray.rows/8, 100, 50, 0, 0);

    // Draw circles detected on image source
    DrawCirclesOnImageSource(circles);

    // Extract circles detected
    FillExtractedCircles(circles);
}

// Circles detection function
void CirclesDetection::DetectCircles(int detection_methode_number)
{
    if(detection_methode_number == 1)
    {
         CirclesDetectionWithoutPretreatment(); // Without image pretreatment
    }
    else if(detection_methode_number == 2)
    {
         CirclesDetectionWithPretreatment(); // With image pretreatment
    }
}

// Get output vector with all extracted circles
std::vector<Mat> CirclesDetection::GetOutputExtractedCircles()
{
     return extractedCircles;
}

// Get the image with all extracted circles drawn
Mat CirclesDetection::GetOutputImage()
{
     return img_source;
}













