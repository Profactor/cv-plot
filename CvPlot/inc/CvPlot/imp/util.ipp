// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/Internal/util.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {
namespace Internal {

namespace Imp {

CVPLOT_DEFINE_FUN
double pixelValue(const cv::Mat &data, int row, int col) {
    cv::Mat1d m;
    data(cv::Rect(col, row, 1, 1)).convertTo(m, m.type());
    return m(0);
}

CVPLOT_DEFINE_FUN
std::string toString(double value, int digits) {
    //TODO scientific
    std::stringstream stream;
    stream.precision(digits);
    stream << std::fixed << value;
    return stream.str();
}

CVPLOT_DEFINE_FUN
bool pixelText(const cv::Mat &data, int row, int col,
    int fontFace, double fontScale, int fontThickness, cv::Size maxSize, std::string &text, cv::Size &size) {
    if (data.channels() > 1) {
        return false; //TODO
    }
    auto checkText = [&](std::string candidate) {
        text = std::move(candidate);
        int baseline;
        size = cv::getTextSize(text, fontFace, fontScale, fontThickness, &baseline);
        return size.width <= maxSize.width && size.height <= maxSize.height;
    };
    switch (data.type()) {
    case CV_16S:
    case CV_16U:
    case CV_32S:
    case CV_8UC1:
    {
        int pix = (int)pixelValue(data, row, col);
        return checkText(std::to_string(pix));
    }
    case CV_32F:
    case CV_64F:
    {
        double pix = pixelValue(data, row, col);
        if (std::isnan(pix)) {
            return checkText("NaN");
        }
        int digits_ = digits(pix);
        for (int i = digits_; i >= 0; i--) {
            if (checkText(toString(pix, i))) {
                return true;
            }
        }
        return false;
    }
    default:
        return false;
    }
}

}

CVPLOT_DEFINE_FUN
int digits(double value) {
	const double epsilon = 0.0001;
	int i;
	for (i = 0; i < 10; i++) {
		double step = std::pow(10, -i);
		double rounded = std::round(value*std::pow(10, i))*step;
		double error = std::abs(value - rounded) / step;
		if (error < epsilon) {
			break;
		}
	}
	return i;
}

CVPLOT_DEFINE_FUN
std::string format(double value, bool exponentional) {
	if (exponentional) {
		int exp = (int)std::log10(value);
		return "1e" + std::to_string(exp);
	}else {
		std::stringstream stream;
		stream.precision(digits(value));
		stream << std::fixed << value;
		return stream.str();
	}
}

CVPLOT_DEFINE_FUN
std::vector<double> calcTicksLinear(double x0, double x1, int estimatedTickCount) {
	if (x1 < x0) {
		std::swap(x0, x1);
	}
	if (estimatedTickCount < 1) {
		estimatedTickCount = 1;
	}
	double step0 = std::abs(x1 - x0) / estimatedTickCount;
	int step0Log = (int)std::ceil(std::log10(step0));
	double step = std::pow(10, step0Log);
	if (step / 5 > step0) {
		step = step / 5;
	}
	else if (step / 2 > step0) {
		step = step / 2;
	}
	const double first = x1 > x0 ? std::ceil(x0 / step)*step : std::floor(x0 / step)*step;
	const int tickCount = (int)std::abs((x1 - first) / step) + 1;
	double signedStep = x1 > x0 ? step : -step;
	std::vector<double> ticks;
	ticks.reserve(tickCount);
	for (int i = 0; i < tickCount; i++) {
		double tick = first + i*signedStep;
		if (tick >= x0 && tick <= x1) {
			ticks.push_back(tick);
		}
	}
	return ticks;
}

CVPLOT_DEFINE_FUN
std::vector<double> calcTicksLog(double x0, double x1, int estimatedTickCount) {
	if (estimatedTickCount < 1) {
		estimatedTickCount = 1;
	}
	if (x1 < x0) {
		std::swap(x0, x1);
	}
	int log0 = (int)std::ceil(std::log10(x0));
	int log1 = (int)std::floor(std::log10(x1));
	std::vector<double> ticks;
	int step = 1 + (int)((log1 - log0) / estimatedTickCount);
	int tickCount = (log1 - log0) / step + 1;
	ticks.resize(tickCount);
	for (int i = 0; i < static_cast<int>(ticks.size()); i++) {
		ticks[i] = std::pow(10, log0 + i * step);
	}
	return ticks;
}

CVPLOT_DEFINE_FUN
std::vector<double> makeX(const std::vector<double>& y) {
	std::vector<double> x(y.size());
	for (size_t i = 0; i < x.size(); i++) {
		x[i] = (double)i;
	}
	return x;
}

CVPLOT_DEFINE_FUN
int drawCast(double value){
	const int max = 32000; //empiric limits because of opencv drawing limitations
	const int min = -32000;
	if (value > max) {
		return max;
	}
	if (value < min) {
		return min;
	}
	return (int)value;
}

CVPLOT_DEFINE_FUN
cv::Rect boundingRect(cv::Rect2d src) {
	normalize(src);
	cv::Rect rect;
	rect.x = (int)std::floor(src.x);
	rect.y = (int)std::floor(src.y);
	rect.width = (int)std::ceil(src.x + src.width) - rect.x;
	rect.height = (int)std::ceil(src.y + src.height) - rect.y;
	return rect;
}

CVPLOT_DEFINE_FUN
void paint(const cv::Mat &src, cv::Mat &dst, const cv::Point &pos) {
	if (src.empty()) {
		return;
	}
	cv::Rect rSrc(0, 0, src.cols, src.rows);
	cv::Rect rDst = rSrc + pos;
	rDst &= cv::Rect(0, 0, dst.cols, dst.rows);
	if (rDst.area() < 1) {
		return;
	}
	rSrc = rDst - pos;
	src(rSrc).copyTo(dst(rDst));
}

CVPLOT_DEFINE_FUN
void paint(const cv::Mat3b &src, cv::Mat3b &dst, const cv::Rect2d &pos, int interpolation, const cv::Mat &data) {
	if (src.empty()) {
		return;
	}
    if (!data.empty() && data.size() != src.size()) {
        throw std::runtime_error("bad data size");
    }
	cv::Rect rSrc(0, 0, src.cols, src.rows);
	cv::Rect rDst(0, 0, dst.cols, dst.rows);
	double kx = pos.width / (double)src.cols; //pixel width
	double dx = pos.x;
	double kxI = 1 / kx;
	double dxI = -kxI*pos.x;
	double ky = pos.height / (double)src.rows; //pixel height
	double dy = pos.y;
	double kyI = 1 / ky; 
	double dyI = -kyI*pos.y;
	cv::Rect2d rDstVis = pos & cv::Rect2d(rDst);
	cv::Rect2d rSrcVis = cv::Rect2d(rDstVis.x*kxI + dxI, rDstVis.y*kyI + dyI, rDstVis.width*kxI, rDstVis.height*kyI);
	cv::Rect rSrcVisEx = boundingRect(rSrcVis);
	rSrcVisEx &= rSrc;
	cv::Rect rDstVisEx = cv::Rect(cv::Rect2d(rSrcVisEx.x*kx + dx, rSrcVisEx.y*ky + dy, rSrcVisEx.width*kx, rSrcVisEx.height*ky));
	if (rSrcVisEx.area() < 1 || rDstVisEx.area()<1) {
		return;
	}
	cv::Mat srcVisEx = src(rSrcVisEx);
	cv::Mat resized;
	cv::resize(srcVisEx, resized, rDstVisEx.size(), 0, 0, interpolation);
	paint(resized, dst, cv::Point(rDstVisEx.x, rDstVisEx.y));

    //grid
    if (kx > 10 && ky > 10) {
        const cv::Rect rDstVisI = cv::Rect(pos) & rDst;
        const auto color = cv::Scalar::all(255);
        for (int c = rSrcVisEx.x; c < rSrcVisEx.x + rSrcVisEx.width; c++) {
            int x = (int)(dx + c * kx);
            cv::line(dst, {x,rDstVisI.y}, {x,rDstVisI.y + rDstVisI.height}, color);
        }
        for (int r = rSrcVisEx.y; r < rSrcVisEx.y + rSrcVisEx.height; r++) {
            int y = (int)(dy + r * ky);
            cv::line(dst, {rDstVisI.x,y}, {rDstVisI.x + rDstVisI.width, y}, color);
        }
    }

    //text
    if (!data.empty() && kx > 20 && ky > 20) {
        const int fontFace = cv::FONT_HERSHEY_SIMPLEX;
        const double fontScale = .4;
        const int fontThickness = 1;
        const cv::Scalar color = cv::Scalar(0, 0, 0);
        const cv::Size maxTextSize((int)kx - 5, (int)ky - 5);
        for (int r = rSrcVisEx.y; r < rSrcVisEx.y+ rSrcVisEx.height; r++) {
            for (int c = rSrcVisEx.x; c < rSrcVisEx.x+ rSrcVisEx.width; c++) {
                std::string text;
                cv::Size textSize;
                if (!Imp::pixelText(data, r, c, fontFace, fontScale, fontThickness, maxTextSize, text, textSize)) {
                    text = "...";
                    int baseline;
                    textSize = cv::getTextSize(text, fontFace, fontScale, fontThickness, &baseline);
                    if (textSize.width > maxTextSize.width || textSize.height > maxTextSize.height) {
                        continue;
                    }
                }
                const cv::Point center((int)(dx + (c+.5) * kx), (int)(dy + (r+.5) * ky));
                const cv::Point textPos(center.x - textSize.width / 2, center.y + textSize.height / 2);
                const cv::Vec3b &bgPix = src(r, c);
                const bool dark = (bgPix(0) + bgPix(1) + bgPix(2)) / 3 < 127;
                const cv::Scalar color = dark ? cv::Scalar::all(255) : cv::Scalar::all(0);
                cv::putText(dst, text, textPos, fontFace, fontScale, color, fontThickness, cv::LINE_AA);
            }
        }
    }
}

}
}