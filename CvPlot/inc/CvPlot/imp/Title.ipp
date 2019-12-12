// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/Title.h>
#include <CvPlot/Internal/util.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {

class Title::Impl {
public:
	std::string _title;
	const int _fontFace = cv::FONT_HERSHEY_SIMPLEX;
	const double _fontScale = .4;
	const int _fontThickness = 1;
	cv::Scalar _color = cv::Scalar(0, 0, 0);

	int getTextWidth(const std::string &text) {
	}
	void render(RenderTarget & renderTarget) {
		cv::Mat3b &outerMat = renderTarget.outerMat();
		auto innerRect = renderTarget.innerRect();
		int xcenter = innerRect.x + innerRect.width / 2;
		int baseline;
		cv::Size size = cv::getTextSize(_title, _fontFace, _fontScale, _fontThickness, &baseline);
		cv::Point titlePos(xcenter - size.width / 2, innerRect.y - (size.height * 3) / 2 );
		cv::putText(outerMat, _title, titlePos, _fontFace, _fontScale, _color, _fontThickness, cv::LINE_AA);
	}
};

CVPLOT_DEFINE_FUN
Title::~Title() {
}

CVPLOT_DEFINE_FUN
Title::Title(const std::string &title){
	setTitle(title);
}

CVPLOT_DEFINE_FUN
void Title::render(RenderTarget & renderTarget){
	impl->render(renderTarget);
}

CVPLOT_DEFINE_FUN
bool Title::getBoundingRect(cv::Rect2d &rect) {
	return false;
}

CVPLOT_DEFINE_FUN
Title & Title::setTitle(const std::string & title){
	impl->_title = title;
	return *this;
}

CVPLOT_DEFINE_FUN
std::string Title::getTitle(){
	return impl->_title;
}
}
