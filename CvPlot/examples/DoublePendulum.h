// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#include <opencv2/opencv.hpp>

//simulation of double pendulum https://en.wikipedia.org/wiki/Double_pendulum
class DoublePendulum {
public:
	void step() {
		double dt1 = 6 / (m*l*l)*(2 * p1 - 3 * cos(t1 - t2)*p2) / (16 - 9 * cos(t1 - t2)*cos(t1 - t2));
		double dt2 = 6 / (m*l*l)*(8 * p2 - 3 * cos(t1 - t2)*p1) / (16 - 9 * cos(t1 - t2)*cos(t1 - t2));
		double dp1 = -.5*m*l*l*(dt1*dt2*sin(t1 - t2) + 3 * g / l * sin(t1));
		double dp2 = -.5*m*l*l*(-dt1 * dt2*sin(t1 - t2) + g / l * sin(t2));
		t1 += dt1 * dt;
		t2 += dt2 * dt;
		p1 += dp1 * dt;
		p2 += dp2 * dt;
		traceVec.push_back(point2());
		if (traceVec.size() > maxTraceSize) {
			traceVec.erase(traceVec.begin());
		}
	}
	cv::Point2d point1() const {
		return { l * sin(t1), l * -cos(t1) };
	}
	cv::Point2d point2() const {
		return point1() + cv::Point2d{ l * sin(t2), l * -cos(t2) };
	}
	const std::vector<cv::Point2d>& trace() const {
		return traceVec;
	}
private:
	double p1 = 0, p2 = 0, t1 = 3, t2 = 3;
	const double m = 1, l = 1, g = 9.81, dt = 1e-2;
	std::vector<cv::Point2d> traceVec;
	const size_t maxTraceSize = 500;
};

