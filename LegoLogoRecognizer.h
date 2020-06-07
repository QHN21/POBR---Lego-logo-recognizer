#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Segment.h"
#include <iostream>

#include <list> 
class LegoLogoRecognizer
{
public:
	
	LegoLogoRecognizer();
	LegoLogoRecognizer(cv::Mat image);
	cv::Mat recognize();
private:
	cv::Mat originalImage;
	cv::Mat processedImage;

	std::vector<Segment> segments;

	static const cv::Scalar MIN_RED;
	static const cv::Scalar MAX_RED;
	static const cv::Scalar MIN_RED_2;
	static const cv::Scalar MAX_RED_2;
	static const cv::Scalar MIN_WHITE;
	static const cv::Scalar MAX_WHITE;

	void filter();
	void convertColor();
	void thresholding();
	void segmentation();
	void deleteSmall();
	void performAnalysis();
	void recognition();
	void floodFill(int row, int col, cv::Scalar color);

	bool isColour(cv::Vec3b pixel, cv::Scalar colourMin, cv::Scalar colourMax);
	bool isLogo(const Segment& s);
};