#include "circlesdetection.hpp"

CirclesDetection::CirclesDetection(Mat img_in, int detection_methode_number):img_source(img_in), detection_methode_number(detection_methode_number)
{}

Mat CirclesDetection::CannyFilter()
{
    Mat dst;
    Mat detected_edges;

    // Reduce noise with a kernel 3x3
    blur(img_source, detected_edges, Size(3,3));

    // Canny detector
    Canny(img_source, detected_edges, 80, 90);

    // Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);
    img_source.copyTo(dst, detected_edges);

    return dst;
}


Mat CirclesDetection::Smooth(Mat img)
{
    Mat smoothed;

    GaussianBlur(img, smoothed, cv::Size(9, 9), 2, 2);

    return smoothed;
}


Mat CirclesDetection::Dilatation(Mat img)
{
    Mat dilated;

    dilate(img, dilated, getStructuringElement(MORPH_ELLIPSE, cv::Size(16, 16)));

    return dilated;
}

Mat CirclesDetection::Erosion(Mat img)
{
    Mat eroded;

    erode(img, eroded, getStructuringElement(MORPH_ELLIPSE, cv::Size(15, 15)));

    return eroded;
}

void CirclesDetection::ConvertToGray(Mat img)
{
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    GaussianBlur(img_gray, img_gray, cv::Size(9, 9),1,1);
}

void CirclesDetection::DrawCirclesOnImageSource(std::vector<Vec3f> circles)
{
        // Draw the circles detected on the original image
	for( size_t i = 0; i < circles.size(); i++ )
	{
                // DrawCircle
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);

		// circle center
		circle(img_source, center, 3, Scalar(0,255,0), -1, 8, 0 );
		// circle outline
		circle(img_source, center, radius, Scalar(0,0,255), 3, 8, 0 );
	}
}

void CirclesDetection::FillExtractedCircles(std::vector<Vec3f> circles)
{
        // Extract the circles detected
	for( size_t i = 0; i < circles.size(); i++ )
	{
                // Extract Circle
		//get the Rect containing the circle:
		Rect r(center.x-radius, center.y-radius, radius*2,radius*2);

		// obtain the image ROI:
                Mat roi(src, r);

		// make a black mask, same size:
		Mat mask(roi.size(), roi.type(), Scalar::all(0));
		// with a white, filled circle in it:
		circle(mask, Point(radius,radius), radius, Scalar::all(255), -1);

		// combine roi & mask:
		Mat circleCropped = roi & mask;

		extractedCircles.push_back(circleCropped);
	}
}

void CirclesDetection::CirclesDetectionWithPreTraitment()
{
    // Image pre traitment
    Mat dst = CannyFilter();
    Mat smoothed = Smooth(dst);
    Mat dilated = Dilatation(smoothed);
    Mat eroded = Erosion(dilated);
    Mat img_gray = ConvertToGray(eroded);

    // Apply the Hough Transform to find the circles
    // CV_HOUGH_GRADIENT : detection method to use (currently, the only implemented one)
    // 1 : inverse ratio of the accumulator resolution to the image resolution
    // src_gray.rows/8 : minimum distance between the centers of the detected circles
    // 200 : the higher threshold of the two passed to the Canny() edge detector (the lower one is twice smaller)
    // 100 : the accumulator threshold for the circle centers at the detection stage. The smaller it is, the more false circles may be detected
    // 0 : Minimum circle radius
    // 0 : Maximum circle radius

    std::vector<Vec3f> circles;
    HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 100, 50, 0, 0);

    // Draw circles detected on image source
    DrawCirclesOnImageSource(circles);

    // Extract circles detected
    FillExtractedCircles(circles);
}


void CirclesDetection::CirclesDetectionWithoutPreTraitment()
{
    // Convert image to Gray
    cvtColor(img_source, img_gray, COLOR_BGR2GRAY);

    // Detect circles
    GaussianBlur(img_gray, img_gray, cv::Size(9, 9), 2, 2);
    HoughCircles(img_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 100, 50, 0, 0);

    // Draw circles detected on image source
    DrawCirclesOnImageSource(circles);

    // Extract circles detected
    FillExtractedCircles(circles);
}


void DetectCircles()
{
    if( detection_methode_number == 0)
    {
         CirclesDetectionWithoutPreTraitment();
    }
    else if( detection_methode_number == 1)
    {
         CirclesDetectionWithPreTraitment();
    }
}

std::vector<Mat> GetOutputExtractedCircles()
{
     return extractedCircles;
}

Mat GetOutputImage()
{
     return img_source;
}













