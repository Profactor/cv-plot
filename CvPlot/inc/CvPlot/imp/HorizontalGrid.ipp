// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/HorizontalGrid.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {

class HorizontalGrid::Impl {
public:
	YAxis *_yAxis = nullptr;
	bool _enabled = true;
	HorizontalGrid &_parent;

	Impl(HorizontalGrid &parent)
		:_parent(parent) {
		_parent.setColor(cv::Scalar::all(200));
	}
	void render(RenderTarget & renderTarget) {
		if (!_enabled || _parent.getLineType()==LineType::None) {
			return;
		}
		if (!_yAxis) {
			return; //TODO: calc ticks
		}
		const auto &ticks = _yAxis->getTicks();
		cv::Mat3b &innerMat = renderTarget.innerMat();
		cv::Scalar color = _parent.getColor();
		int lineWidth = _parent.getLineWidth();
		for (double tick : ticks) {
			int tickPix = (int)(renderTarget.project(cv::Point2d(0,tick)).y + .5);
			for (int x = 0; x < innerMat.cols; x += 8) {
				cv::line(innerMat, cv::Point(x, tickPix), cv::Point(x + 4, tickPix), color, lineWidth);
			}
			cv::line(innerMat, cv::Point(0, tickPix), cv::Point(5, tickPix), color, lineWidth);
		}
	}
};

CVPLOT_DEFINE_FUN
HorizontalGrid::HorizontalGrid(YAxis* yAxis) 
	:impl(*this){
	impl->_yAxis = yAxis;
}

CVPLOT_DEFINE_FUN
HorizontalGrid::~HorizontalGrid() {
}

CVPLOT_DEFINE_FUN
HorizontalGrid & HorizontalGrid::setYAxis(YAxis * yAxis) {
	impl->_yAxis = yAxis;
	return *this;
}

CVPLOT_DEFINE_FUN
void HorizontalGrid::render(RenderTarget & renderTarget){
	impl->render(renderTarget);
}

CVPLOT_DEFINE_FUN
bool HorizontalGrid::getBoundingRect(cv::Rect2d &rect) {
	return false;
}

CVPLOT_DEFINE_FUN
HorizontalGrid & HorizontalGrid::setEnabled(bool enabled){
	impl->_enabled = enabled;
	return *this;
}

CVPLOT_DEFINE_FUN
bool HorizontalGrid::getEnabled(){
	return impl->_enabled;
}
}
