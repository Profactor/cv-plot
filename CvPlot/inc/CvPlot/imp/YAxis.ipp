// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/YAxis.h>
#include <CvPlot/Internal/util.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {

class YAxis::Impl {
public:
	const int _fontFace = cv::FONT_HERSHEY_SIMPLEX;
	const double _fontScale = .4;
	const int _fontThickness = 1;
	bool _locateRight = false;
	cv::Scalar _color = cv::Scalar(0, 0, 0);
	std::vector<double> _ticks;
	bool _isLogarithmic;
	int _width = 0;

	void calcTicks(const RenderTarget & renderTarget) {
		cv::Rect innerRect = renderTarget.innerRect();
		double y0 = renderTarget.unproject(cv::Point2d(0, 0)).y;
		double y05 = renderTarget.unproject(cv::Point2d(0, innerRect.height/2.)).y;
		double y1 = renderTarget.unproject(cv::Point2d(0, innerRect.height)).y;
		if (y1 == y0 || !std::isfinite(y0) || !std::isfinite(y05) || !std::isfinite(y1)) {
			_ticks = { y0 };
			return;
		}
		if (y1 < y0) {
			std::swap(y0, y1);
		}
		int labelHeight = getTextSize("1,2").height;
		int spacing = 20;
		int estimatedTickCount = (int)std::ceil(innerRect.height / (labelHeight + spacing));
		double epsilon = 1e-5;
		_isLogarithmic = std::abs(2 * (y05 - y0) / (y1 - y0) - 1) > epsilon;
		if (_isLogarithmic) {
			_ticks = Internal::calcTicksLog(y0, y1, estimatedTickCount);
		} else {
			_ticks = Internal::calcTicksLinear(y0, y1, estimatedTickCount);
		}
	}
	cv::Size getTextSize(const std::string &text) {
		int baseline;
		return cv::getTextSize(text, _fontFace, _fontScale, _fontThickness, &baseline);
	}
	void render(RenderTarget & renderTarget) {
		cv::Mat3b &outerMat = renderTarget.outerMat();
		const cv::Rect &innerRect = renderTarget.innerRect();
		if (!innerRect.area()) {
			return;
		}
		calcTicks(renderTarget);
		_width = 0;
		for (double tick : _ticks) {
			int tickPix = (int)(renderTarget.project(cv::Point2d(0, tick)).y + .5);
			std::string label = Internal::format(tick, _isLogarithmic);
			cv::Size size = getTextSize(label);
			cv::Point labelPos;
			cv::Point tickPos;
			const int margin = 10;
			const int tickLength = 4;
			if (!_locateRight) {
				labelPos.x = innerRect.x - margin - size.width;
				tickPos.x = innerRect.x - tickLength;
			} else {
				labelPos.x = innerRect.x + innerRect.width + margin;
				tickPos.x = innerRect.x + innerRect.width;
			}
			labelPos.y = (int)(innerRect.y + tickPix + size.height / 2);
			tickPos.y = innerRect.y + tickPix;

			cv::putText(outerMat, label, labelPos, _fontFace, _fontScale, _color, _fontThickness, cv::LINE_AA);
			cv::line(outerMat, tickPos, tickPos + cv::Point(tickLength-1,0), _color);
			if (margin + size.width > _width) {
				_width = margin + size.width;
			}
		}
		renderAxisLine(renderTarget);
	}
	void renderAxisLine(RenderTarget & renderTarget){
		cv::Mat3b &outerMat = renderTarget.outerMat();
		const cv::Rect &innerRect = renderTarget.innerRect();
		int top = innerRect.y - 1;
		int bottom = innerRect.y + innerRect.height;
		int x = innerRect.x - 1;
		cv::line(outerMat, cv::Point(x, top), cv::Point(x, bottom), _color);
	}
};

CVPLOT_DEFINE_FUN
YAxis::~YAxis() {
}

CVPLOT_DEFINE_FUN
YAxis::YAxis(){
}

CVPLOT_DEFINE_FUN
void YAxis::render(RenderTarget & renderTarget){
	impl->render(renderTarget);
}

CVPLOT_DEFINE_FUN
const std::vector<double> & YAxis::getTicks() const{
	return impl->_ticks;
}

CVPLOT_DEFINE_FUN
int YAxis::getWidth()const {
	return impl->_width;
}

CVPLOT_DEFINE_FUN
bool YAxis::getBoundingRect(cv::Rect2d &rect) {
	return false;
}

CVPLOT_DEFINE_FUN
YAxis& YAxis::setLocateRight(bool locateRight) {
	impl->_locateRight = locateRight;
	return *this;
}

}
