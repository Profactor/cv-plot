// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/core/Axes.h>
#include <opencv2/highgui.hpp>
#include <functional>

namespace CvPlot {

class MouseEvent {
public:
	//renderSize: size of rendered cv::Mat
	//rest of args: see cv::setMouseCallback documentation
	MouseEvent(Axes &axes, cv::Size renderSize, int event, int x, int y, int flags)
		:_axes(axes)
		, _renderSize(renderSize)
		, _event(event)
		, _x(x)
		, _y(y)
		, _flags(flags) {
	}
	Axes &axes() const{
		return _axes;
	}
	//size of rendered cv::Mat
	cv::Size renderSize() const {
		return _renderSize;
	}
	//event type (see cv::setMouseCallback documentation)
	int event()const {
		return _event;
	}
	int x()const {
		return _x;
	}
	int y()const {
		return _y;
	}
	//flags (see cv::setMouseCallback documentation)
	int flags()const {
		return _flags;
	}
	Projection projection()const {
		return _axes.getProjection(_renderSize);
	}
	//position in outer rect
	cv::Point outerPoint()const {
		return cv::Point(_x, _y);
	}
	//position in inner rect
	cv::Point innerPoint()const {
		auto outer = outerPoint();
		auto innerRect = projection().innerRect();
		return cv::Point(outer.x - innerRect.x, outer.y - innerRect.y);
	}
	//position in logical coordinates
	cv::Point2d pos()const {
		return projection().unproject(innerPoint());
	}

private:
	Axes &_axes;
	cv::Size _renderSize;
	int _event;
	int _x;
	int _y;
	int _flags;
};

//return: true if the Axes needs to re-render
typedef std::function<bool(const MouseEvent&)> MouseEventHandler;

}
