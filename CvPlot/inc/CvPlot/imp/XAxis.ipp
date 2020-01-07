// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/XAxis.h>
#include <CvPlot/Internal/util.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {

class XAxis::Impl {
public:
	const int _fontFace = cv::FONT_HERSHEY_SIMPLEX;
	const double _fontScale = .4;
	const int _fontThickness = 1;
	cv::Scalar _color = cv::Scalar(0, 0, 0);
	std::vector<double> _ticks;
	bool _isLogarithmic;

	int estimateLabelWidth(double x, double step) {
		std::string label;
		if (_isLogarithmic) {
			label = Internal::format(x, true);
		}else {
			int digits = std::max(0, -(int)(std::floor(std::log10(step))));
			double rounded = std::floor(x*std::pow(10, digits))*std::pow(10, -digits);
			label = Internal::format(rounded);
		}
		return getTextWidth(label);
	}

	void calcTicks(const RenderTarget & renderTarget) {
		cv::Rect innerRect = renderTarget.innerRect();
		double x0 = renderTarget.unproject(cv::Point2d(0, 0)).x;
		double x05 = renderTarget.unproject(cv::Point2d(innerRect.width/2., 0)).x;
		double x1 = renderTarget.unproject(cv::Point2d(innerRect.width, 0)).x;
		if (x1 == x0 || !std::isfinite(x0) || !std::isfinite(x05) || !std::isfinite(x1)) {
			_ticks = { x0 };
			return;
		}
		if (x1 < x0) {
			std::swap(x0, x1);
		}
		double epsilon = 1e-5;
		_isLogarithmic = std::abs(2 * (x05 - x0) / (x1 - x0) - 1) > epsilon;
		double step0 = (x1 - x0) / 10;
		int estimatedLabelWidth = std::max(estimateLabelWidth(x0, step0), estimateLabelWidth(x1,step0));
		int spacing = 30;
		int estimatedTickCount = (int)std::ceil(innerRect.width / (estimatedLabelWidth + spacing));
		if (_isLogarithmic) {
			_ticks = Internal::calcTicksLog(x0, x1, estimatedTickCount);
		} else {
			_ticks = Internal::calcTicksLinear(x0, x1, estimatedTickCount);
		}
	}
	int getTextWidth(const std::string &text) {
		int baseline;
		return cv::getTextSize(text, _fontFace, _fontScale, _fontThickness, &baseline).width;
	}
	void render(RenderTarget & renderTarget) {
		cv::Mat3b &outerMat = renderTarget.outerMat();
		const cv::Rect &innerRect = renderTarget.innerRect();
		if (!innerRect.area()) {
			return;
		}
		calcTicks(renderTarget);
		const int bottom = innerRect.y + innerRect.height;
		cv::Point labelPos;
		labelPos.y = bottom + 20;
		for (double tick : _ticks) {
			int tickPix = (int)(renderTarget.project(cv::Point2d(tick, 0)).x + .5);
			std::string label = Internal::format(tick, _isLogarithmic);
			int w = getTextWidth(label);
			labelPos.x = (int)(innerRect.x + tickPix - w / 2);
			cv::putText(outerMat, label, labelPos, _fontFace, _fontScale, _color, _fontThickness, cv::LINE_AA);
			cv::line(outerMat, cv::Point(tickPix+innerRect.x, bottom), cv::Point(tickPix + innerRect.x, bottom+4), _color);
		}
		renderAxisLine(renderTarget);
	}
	void renderAxisLine(RenderTarget & renderTarget){
		cv::Mat3b &outerMat = renderTarget.outerMat();
		const cv::Rect &innerRect = renderTarget.innerRect();
		int left = innerRect.x - 1;
		int right = innerRect.x + innerRect.width;
		int y = innerRect.y + innerRect.height;
		cv::line(outerMat, cv::Point(left, y), cv::Point(right, y), _color);
	}
};

CVPLOT_DEFINE_FUN
XAxis::~XAxis() {
}

CVPLOT_DEFINE_FUN
XAxis::XAxis(){
}

CVPLOT_DEFINE_FUN
void XAxis::render(RenderTarget & renderTarget){
	impl->render(renderTarget);
}

CVPLOT_DEFINE_FUN
const std::vector<double> & XAxis::getTicks() {
	return impl->_ticks;
}

CVPLOT_DEFINE_FUN
bool XAxis::getBoundingRect(cv::Rect2d &rect) {
	return false;
}
}
