#include <iostream>



#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "LegoLogoRecognizer.h"

int main(char* []) {
	char filename[50];
	LegoLogoRecognizer recognizer;
	cv::Mat image;
	for (int i = 1; i < 12; i++) {
		sprintf_s(filename, "obrazy/LEGO_%d.jpg", i);
		image = cv::imread(filename);

		//cv::Mat image = cv::imread("obrazy/LEGO_LOGO_compressed.jpg");
		//cv::Mat image = cv::imread("obrazy/LEGO_5_2.jpg");

		//cv::Mat image = cv::imread("obrazy/Lego_5_compressed.jpg");

		if (image.empty())
		{
			std::cout << "This image is empty!" << std::endl;
			return 1;
		}
		recognizer = LegoLogoRecognizer(image);

		image = recognizer.recognize();
		sprintf_s(filename, "obrazy/LEGO_%d_processed.jpg", i);
		cv::imwrite(filename, image);
	}
	//LogoRecognizer lg = LogoRecognizer(image);
	//lg.recognize();
	cv::waitKey(-1);
	return 0;
}