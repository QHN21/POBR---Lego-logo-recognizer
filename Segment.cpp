#include "Segment.h"

Segment::Segment()
{
	color = cv::Scalar(255, 255, 255);
}

Segment::Segment(cv::Scalar _color)
{
	color = _color;
}

double Segment::calc_mpq(int p, int q) {
	double moment = 0;
	for (std::vector<cv::Point2i>::iterator it = points.begin(); it != points.end(); ++it) {
		moment += pow(it->y + 1, p) * pow(it->x + 1, q);
	}
	return moment;
}

bool Segment::operator==(const Segment& s)
{
	return (color.val[0] == s.color.val[0] && color.val[1] == s.color.val[1] && color.val[2] == s.color.val[2]);
}


void Segment::analyseSegment() {
	area = points.size();

	m01 = calc_mpq(0, 1);
	m10 = calc_mpq(1, 0);
	m02 = calc_mpq(0, 2);
	m20 = calc_mpq(2, 0);
	m03 = calc_mpq(0, 3);
	m30 = calc_mpq(3, 0);
	m11 = calc_mpq(1, 1);
	m21 = calc_mpq(2, 1);
	m12 = calc_mpq(1, 2);

	center_i = m10 / area;
	center_j = m01 / area;

	M01 = m01 - (m01 / area) * area;
	M10 = m10 - (m10 / area) * area;
	M11 = m11 - m10 * m01 / area;
	M20 = m20 - m10 * m10 / area;
	M02 = m02 - m01 * m01 / area;
	M21 = m21 - 2 * m11 * center_i - m20 * center_j + 2 * m01 * center_i * center_i;
	M12 = m12 - 2 * m11 * center_j - m02 * center_i + 2 * m10 * center_j * center_j;
	M30 = m30 - 3 * m20 * center_i + 2 * m10 * center_i * center_i;
	M03 = m03 - 3 * m02 * center_j + 2 * m01 * center_j * center_j;

	NM1 = (M20 + M02) / pow(area, 2);
	NM2 = ((M20 - M02) * (M20 - M02) + 4 * M11 * M11) / pow(area, 4);
	NM3 = ((M30 - 3 * M12) * (M30 - 3 * M12) + (3 * M21 - M03) * (3 * M21 - M03)) / pow(area, 5);
	NM7 = (M20 * M02 - M11 * M11) / pow(area, 4);
}
