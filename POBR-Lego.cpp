#include <iostream>



#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "LegoLogoRecognizer.h"

int main(char* []) {
	char filename[50];
	LegoLogoRecognizer recognizer;
	cv::Mat image;
	for (int i = 1; i < 10; i++) {
		sprintf_s(filename, "obrazy/LEGO_%d.jpg", i);
		image = cv::imread(filename);
		if (image.empty())
		{
			std::cout << "This image is empty!" << std::endl;
			break;
		}
		recognizer = LegoLogoRecognizer(image);
		image = recognizer.recognize();
		sprintf_s(filename, "obrazy/LEGO_%d_processed.jpg", i);
		cv::imwrite(filename, image);
	}
	cv::waitKey(-1);
	return 0;
}