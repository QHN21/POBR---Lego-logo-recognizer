#include <iostream>



#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "LegoLogoRecognizer.h"

int main(char* []) {
	//cv::Mat image = cv::imread("obrazy/LEGO_LOGO.jpg");

	//cv::Mat image = cv::imread("obrazy/LEGO_LOGO_compressed.jpg");
	cv::Mat image = cv::imread("obrazy/LEGO_5_2.jpg");

	//cv::Mat image = cv::imread("obrazy/Lego_5_compressed.jpg");

	if (image.empty())
	{
		std::cout << "This image is empty!" << std::endl;
		return 1;
	}
	LegoLogoRecognizer recognizer = LegoLogoRecognizer(image);

	recognizer.recognize();

	//LogoRecognizer lg = LogoRecognizer(image);
	//lg.recognize();
	cv::waitKey(-1);
	return 0;
}