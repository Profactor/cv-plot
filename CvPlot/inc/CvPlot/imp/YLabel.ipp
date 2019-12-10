// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/YLabel.h>
#include <CvPlot/Internal/util.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {

class YLabel::Impl {
public:
	std::string _label;
	const int _fontFace = cv::FONT_HERSHEY_SIMPLEX;
	const double _fontScale = .4;
	const int _fontThickness = 1;
	cv::Scalar _color = cv::Scalar(0, 0, 0);
	YAxis* _yAxis = nullptr;

	void render(RenderTarget & renderTarget) {
		cv::Mat3b &outerMat = renderTarget.outerMat();
		cv::Rect &innerRect = renderTarget.innerRect();
		if (!innerRect.area() || _label.empty()) {
			return;
		}
		int left = innerRect.x;
		int ycenter = innerRect.y + innerRect.height / 2;
		int baseline;
		cv::Size textSize = cv::getTextSize(_label, _fontFace, _fontScale, _fontThickness, &baseline);
		int margin = _fontThickness + 5; //required for cv::LINE_AA
		cv::Mat3b temp(textSize.height + 2 * margin, textSize.width + baseline + 2 * margin, cv::Vec3b::all(255));
		cv::Point pos((temp.cols - textSize.width) / 2, textSize.height + margin);
		cv::putText(temp, _label, pos, _fontFace, _fontScale, _color, _fontThickness, cv::LINE_AA);
		temp = temp.t();
		cv::flip(temp, temp, 0);
		int yAxisWidth = _yAxis ? _yAxis->getWidth() : 0;
		cv::Point labelTopLeft(left - yAxisWidth - 5 - temp.cols, ycenter - temp.rows / 2);
		Internal::paint(temp, outerMat, labelTopLeft);
	}
};

CVPLOT_DEFINE_FUN
YLabel::~YLabel() {
}

CVPLOT_DEFINE_FUN
YLabel::YLabel(const std::string &label, YAxis* yAxis) {
	impl->_yAxis = yAxis;
	setLabel(label);
}

CVPLOT_DEFINE_FUN
void YLabel::render(RenderTarget & renderTarget){
	impl->render(renderTarget);
}

CVPLOT_DEFINE_FUN
bool YLabel::getBoundingRect(cv::Rect2d &rect) {
	return false;
}

CVPLOT_DEFINE_FUN
YLabel & YLabel::setLabel(const std::string & label){
	impl->_label = label;
	return *this;
}

CVPLOT_DEFINE_FUN
std::string YLabel::getLabel(){
	return impl->_label;
}

CVPLOT_DEFINE_FUN
YLabel& YLabel::setYAxis(YAxis* yAxis) {
	impl->_yAxis = yAxis;
	return *this;
}

}
