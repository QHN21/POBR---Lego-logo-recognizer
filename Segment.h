#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <list>

class Segment
{
public:
	double m01, m10, m02, m20, m03, m30, m11, m21, m12;
    double center_i, center_j;
    double M01, M10, M02, M20, M03, M30,M11, M21, M12;
    double NM1, NM2, NM3, NM7;
	std::vector<cv::Point2i> points;
	cv::Scalar color;
	double area;
	Segment();
	Segment(cv::Scalar _color);
	bool operator==(const Segment& q);
	void analyseSegment();
private:
	double calc_mpq(int p, int q);
};

