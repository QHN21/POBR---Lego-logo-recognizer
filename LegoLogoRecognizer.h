#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

class LegoLogoRecognizer
{
public:
	LegoLogoRecognizer(cv::Mat image);
	void recognize();
private:
	cv::Mat originalImage;
	cv::Mat processedImage;

	static const cv::Scalar MIN_RED;
	static const cv::Scalar MAX_RED;

	static const cv::Scalar MIN_RED_2;
	static const cv::Scalar MAX_RED_2;
	static const cv::Scalar MIN_WHITE;
	static const cv::Scalar MAX_WHITE;

	void convertColor(cv::Mat image);
	void thresholding(cv::Mat image);
	bool isColour(cv::Vec3b pixel, cv::Scalar colourMin, cv::Scalar colourMax);
};