// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/drawables/VerticalGrid.h>
#include <opencv2/opencv.hpp>

namespace CvPlot {

class VerticalGrid::Impl {
public:
	XAxis* _xAxis = nullptr;
	bool _enabled = true;
	VerticalGrid &_parent;

	Impl(VerticalGrid &parent)
		:_parent(parent) {
		_parent.setColor(cv::Scalar::all(200));
	}

	void render(RenderTarget & renderTarget) {
		if (!_enabled || _parent.getLineType() == LineType::None) {
			return;
		}
		if (!_xAxis) {
			return; //TODO: calc ticks
		}
		const auto &ticks = _xAxis->getTicks();
		cv::Mat3b &innerMat = renderTarget.innerMat();
		cv::Scalar color = _parent.getColor();
		int lineWidth = _parent.getLineWidth();
		for (double tick : ticks) {
			int tickPix = (int)(renderTarget.project(cv::Point2d(tick, 0)).x + .5);
			for (int y = 0; y < innerMat.rows; y += 8) {
				cv::line(innerMat, cv::Point(tickPix, y), cv::Point(tickPix, y + 4), color, lineWidth);
			}
			cv::line(innerMat, cv::Point(tickPix, innerMat.rows - 5), cv::Point(tickPix, innerMat.rows), color, lineWidth);
		}
	}
};

CVPLOT_DEFINE_FUN
VerticalGrid::VerticalGrid(XAxis* xAxis)
	:impl(*this) {
	impl->_xAxis = xAxis;
}

CVPLOT_DEFINE_FUN
VerticalGrid::~VerticalGrid() {
}

CVPLOT_DEFINE_FUN
VerticalGrid& VerticalGrid::setXAxis(XAxis * xAxis) {
	impl->_xAxis = xAxis;
	return *this;
}

CVPLOT_DEFINE_FUN
void VerticalGrid::render(RenderTarget & renderTarget){
	impl->render(renderTarget);
}

CVPLOT_DEFINE_FUN
bool VerticalGrid::getBoundingRect(cv::Rect2d &rect) {
	return false;
}

CVPLOT_DEFINE_FUN
VerticalGrid & VerticalGrid::setEnabled(bool enabled) {
	impl->_enabled = enabled;
	return *this;
}

CVPLOT_DEFINE_FUN
bool VerticalGrid::getEnabled() {
	return impl->_enabled;
}

}
