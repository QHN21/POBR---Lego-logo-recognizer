#include "LegoLogoRecognizer.h"


const cv::Scalar LegoLogoRecognizer::MIN_RED = cv::Scalar(0, 100, 100);
const cv::Scalar LegoLogoRecognizer::MAX_RED = cv::Scalar(4, 255, 255);

const cv::Scalar LegoLogoRecognizer::MIN_RED_2 = cv::Scalar(174, 100, 100);
const cv::Scalar LegoLogoRecognizer::MAX_RED_2 = cv::Scalar(180, 255, 255);
const cv::Scalar LegoLogoRecognizer::MIN_WHITE = cv::Scalar(0, 0, 150);
const cv::Scalar LegoLogoRecognizer::MAX_WHITE = cv::Scalar(180, 70, 255);

LegoLogoRecognizer::LegoLogoRecognizer(cv::Mat image) {
	originalImage = image.clone();
	processedImage = image.clone();
}

void LegoLogoRecognizer::recognize() {
	convertColor(processedImage);
	thresholding(processedImage);
	cv::namedWindow("Lego", cv::WINDOW_NORMAL);
	cv::imshow("Lego", originalImage);


	cv::namedWindow("Processed", cv::WINDOW_NORMAL);
	cv::imshow("Processed", processedImage);
}

void LegoLogoRecognizer::convertColor(cv::Mat image) {
	for (int i = 0; i < image.rows; ++i) {
		for (int j = 0; j < image.cols; ++j) {
			cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
			const uchar red = pixel[2], green = pixel[1], blue = pixel[0];
			const uchar Cmax = std::max(std::max(red, green), blue);
			const uchar Cmin = std::min(std::min(red, green), blue);
			const uchar delta = Cmax - Cmin;
			const uchar value = Cmax;

			uchar saturation = Cmax == 0 ? 0 : delta * 255 / Cmax;
			
		
			int hue;
			if (Cmin == value)
				hue = 0;
			else if (red == Cmax)
				hue = (((green - blue) * 60 / delta));
			else if (green == Cmax)
				hue = ((blue - red) * 60 / delta) + 120;
			else if (blue == Cmax)
				hue = ((red - green) * 60 / delta) + 240;
			
			if (hue < 0)
				hue += 360;


			pixel = { static_cast<uchar>(hue / 2), saturation, value };
			image.at<cv::Vec3b>(i, j) = pixel;
		}
	}
	return;
}


void LegoLogoRecognizer::thresholding(cv::Mat image) {
	for (int i = 0; i < image.rows; ++i) {
		for (int j = 0; j < image.cols; ++j) {
			cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
			if (isColour(pixel, MIN_RED, MAX_RED) || isColour(pixel, MIN_RED_2, MAX_RED_2) || isColour(pixel, MIN_WHITE, MAX_WHITE)) {
				pixel = { 255, 255, 255 };
			}else{
				pixel = { 0, 0, 0 };
			}
			image.at<cv::Vec3b>(i, j) = pixel;
		}
	}
	return;
}


bool LegoLogoRecognizer::isColour(cv::Vec3b pixel, cv::Scalar colourMin, cv::Scalar colourMax) {
	if ((pixel[0] <= colourMax[0] && pixel[0] >= colourMin[0]) && (pixel[1] <= colourMax[1] && pixel[1] >= colourMin[1]) && (pixel[2] <= colourMax[2] && pixel[2] >= colourMin[2])) {
		return true;
	}
	else {
		return false;
	}
}


