#include "LegoLogoRecognizer.h"


const cv::Scalar LegoLogoRecognizer::MIN_RED = cv::Scalar(0, 50, 50);
const cv::Scalar LegoLogoRecognizer::MAX_RED = cv::Scalar(6, 255, 255);

const cv::Scalar LegoLogoRecognizer::MIN_RED_2 = cv::Scalar(174, 50, 50);
const cv::Scalar LegoLogoRecognizer::MAX_RED_2 = cv::Scalar(180, 255, 255);
const cv::Scalar LegoLogoRecognizer::MIN_WHITE = cv::Scalar(0, 0, 150);
const cv::Scalar LegoLogoRecognizer::MAX_WHITE = cv::Scalar(180, 70, 255);

LegoLogoRecognizer::LegoLogoRecognizer() {

}

LegoLogoRecognizer::LegoLogoRecognizer(cv::Mat image) {
	originalImage = image.clone();
	processedImage = image.clone();
}

cv::Mat LegoLogoRecognizer::recognize() {
	convertColor();
	thresholding();
	segmentation();
	deleteSmall();
	performAnalysis();
	recognition();
	cv::namedWindow("Lego", cv::WINDOW_NORMAL);
	cv::imshow("Lego", originalImage);

	cv::namedWindow("Processed", cv::WINDOW_NORMAL);
	cv::imshow("Processed", processedImage);
	return originalImage;
}

void LegoLogoRecognizer::convertColor() {
	for (int i = 0; i < processedImage.rows; ++i) {
		for (int j = 0; j < processedImage.cols; ++j) {
			cv::Vec3b pixel = processedImage.at<cv::Vec3b>(i, j);
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
			processedImage.at<cv::Vec3b>(i, j) = pixel;
		}
	}
	return;
}


void LegoLogoRecognizer::thresholding() {
	for (int i = 0; i < processedImage.rows; ++i) {
		for (int j = 0; j < processedImage.cols; ++j) {
			cv::Vec3b pixel = processedImage.at<cv::Vec3b>(i, j);
			if (isColour(pixel, MIN_RED, MAX_RED) || isColour(pixel, MIN_RED_2, MAX_RED_2)) {    //  || isColour(pixel, MIN_WHITE, MAX_WHITE)
				pixel = { 255, 255, 255 };
			}else{
				pixel = { 0, 0, 0 };
			}
			processedImage.at<cv::Vec3b>(i, j) = pixel;
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


void LegoLogoRecognizer::segmentation() {
	int n = 2;
	cv::Scalar color;
	for (int i = 0; i < processedImage.rows; ++i) {
		for (int j = 0; j < processedImage.cols; ++j) {
	
			cv::Vec3b pixel = processedImage.at<cv::Vec3b>(i, j);
			if (pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255) {
				cv::RNG rng(n);
				color = cv::Scalar((unsigned)rng%256, ((unsigned)rng>> 2) % 256, ((unsigned)rng>> 4) % 256);
				segments.push_back(Segment(color));
				floodFill(i, j, color);
				++n;
			}
		}
		
	}
}

void LegoLogoRecognizer::floodFill(int row, int col, cv::Scalar color) {
	std::list<cv::Point2i> pointQueue;
	pointQueue.push_back(cv::Point2i(col, row));
	cv::Point2i temp, left, right, top, bottom;

	while (!pointQueue.empty()) {
		//pobranie pierwszego elementu z kolejki
	
		temp = pointQueue.front();
		pointQueue.pop_front();

		cv::Vec3b pixel = processedImage.at<cv::Vec3b>(temp);
		//zamalowanie piksela w obrazie na wybrany kolor
		pixel[0] = (uchar)color[0];
		pixel[1] = (uchar)color[1];
		pixel[2] = (uchar)color[2];

		processedImage.at<cv::Vec3b>(temp) = pixel;
		//dodanie punktu do segmentu
		segments.back().points.push_back(temp);

		left = cv::Point2i(temp.x - 1, temp.y);
		right = cv::Point2i(temp.x + 1, temp.y);
		top = cv::Point2i(temp.x, temp.y - 1);
		bottom = cv::Point2i(temp.x, temp.y + 1);

		/* jesli wsp. sasiadow mieszcza sie w zakresie, w binarnym obrazie jest bialy piksel
		i nie ma juz w kolejce punktu o takich wspolrzednych,
		to wstaw punkt do kolejki*/
		if (left.x >= 0) {
			pixel = processedImage.at<cv::Vec3b>(left);
			if (left.x >= 0 && pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255 &&
				find(pointQueue.begin(), pointQueue.end(), left) == pointQueue.end()) {
				pointQueue.push_back(left);
			}
		}
		if (right.x < processedImage.cols) {
			pixel = processedImage.at<cv::Vec3b>(right);
			if (right.x < processedImage.cols && pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255 &&
				find(pointQueue.begin(), pointQueue.end(), right) == pointQueue.end()) {
				pointQueue.push_back(right);
			}
		}

		if (top.y >= 0) {
			pixel = processedImage.at<cv::Vec3b>(top);
			if (top.y >= 0 && pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255 &&
				find(pointQueue.begin(), pointQueue.end(), top) == pointQueue.end()) {
				pointQueue.push_back(top);
			}
		}
	
		if (bottom.y < processedImage.rows) {
			pixel = processedImage.at<cv::Vec3b>(bottom);
			if (bottom.y < processedImage.rows && pixel[0] == 255 && pixel[1] == 255 && pixel[2] == 255 &&
				find(pointQueue.begin(), pointQueue.end(), bottom) == pointQueue.end()) {
				pointQueue.push_back(bottom);
			}
		}
	}
}

void LegoLogoRecognizer::deleteSmall() {
	cv::Scalar black = cv::Scalar(0, 0, 0);
	std::cout << segments.size() << std::endl;
	std::vector<Segment>::iterator it = segments.begin();
	while (it != segments.end()) {
		it->area = it->points.size();
		if (it->area < 300) {
			it->color = black;
			for (std::vector<cv::Point2i>::iterator i = it->points.begin(); i != it->points.end(); ++i) {
				processedImage.at<cv::Vec3b>(*i)[0] = 0;
				processedImage.at<cv::Vec3b>(*i)[1] = 0;
				processedImage.at<cv::Vec3b>(*i)[2] = 0;
			}
			it = segments.erase(it);
		}
		else {
			++it;
		}
	}
	std::cout << segments.size() << std::endl;
}

void LegoLogoRecognizer::performAnalysis() {
	for (std::vector<Segment>::iterator it = segments.begin(); it != segments.end(); ++it) {
		it->analyseSegment();
		std::cout << it->NM1 <<"  " << it->NM2 << "  " << it->NM3 << "  " << it->NM7 << std::endl;
	}
}

void LegoLogoRecognizer::recognition() {
	std::vector<cv::Point2i>  minBackground;
	std::vector<cv::Point2i>  maxBackground;
	int xMin, yMin, xMax, yMax;
	for (std::vector<Segment>::iterator it = segments.begin(); it != segments.end(); ++it) {
		if (isBackground(*it)) {
			yMax = xMax = 0;
			xMin =processedImage.cols;
			yMin = processedImage.rows;
			for (std::vector<cv::Point2i>::iterator i = it->points.begin(); i != it->points.end(); ++i) {
				if (xMin > i->x)
					xMin = i->x;
				if (yMin > i->y)
					yMin = i->y;
				if (xMax < i->x)
					xMax = i->x;
				if (yMax < i->y)
					yMax = i->y;
			}
			cv::rectangle(originalImage, cv::Point2i(xMin-2,yMin - 2), cv::Point2i(xMax + 2, yMax + 2), cv::Scalar(0, 255, 0), +2, 4);

			
		}
		else {
			for (std::vector<cv::Point2i>::iterator i = it->points.begin(); i != it->points.end(); ++i) {
				processedImage.at<cv::Vec3b>(*i)[0] = 0;
				processedImage.at<cv::Vec3b>(*i)[1] = 0;
				processedImage.at<cv::Vec3b>(*i)[2] = 0;
			}
		}


		
	}
}

bool LegoLogoRecognizer::isBackground(const Segment& s) {
	double NM1 = s.NM1;
	double NM2 = s.NM2;
	double NM3 = s.NM3;
	double NM7 = s.NM7;
	double NM1_max = 0.39;
	double NM1_min = 0.32;

	double NM2_max = 0.005;
	double NM2_min = 0.0002;

	double NM3_max = 0.0009;
	double NM3_min = 0.0000001;
	
	double NM7_max = 0.049;
	double NM7_min = 0.009;
	
	if ((NM1_min < NM1 && NM1 < NM1_max) && (NM2_min < NM2 && NM2 < NM2_max) && (NM3_min < NM3 && NM3 < NM3_max) && (NM7_min < NM7 && NM7 < NM7_max)) {
		return true;
	}
	return false;
}
