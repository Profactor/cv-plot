// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/gui/MouseAdapter.h>

namespace CvPlot {

class MouseAdapter::Impl {
public:
	Axes &_axes;
	cv::Point _pressPosPix, _lastPosPix;
	MouseEventHandler _mouseEventHandler;

	Impl(Axes &axes)
		:_axes(axes) {
	}
	bool mouseEvent(const MouseEvent &mouseEvent) {
		bool update = false;
		if (handleEvent(mouseEvent)) {
			update = true;
		}
		if (_mouseEventHandler && _mouseEventHandler(mouseEvent)){
			update = true;
		}
		return update;
	}
private:
	bool handleEvent(const MouseEvent &mouseEvent){
		if (mouseEvent.event() == cv::EVENT_MBUTTONDOWN
			|| mouseEvent.event() == cv::EVENT_RBUTTONDOWN) {
			_pressPosPix = mouseEvent.outerPoint();
			_lastPosPix = _pressPosPix;
			return true;
		}
		if (mouseEvent.event() == cv::EVENT_MOUSEMOVE) {
			cv::Point deltaPix = mouseEvent.outerPoint() - _lastPosPix;
			if (mouseEvent.flags() & cv::EVENT_FLAG_MBUTTON) {
				_axes.pan(mouseEvent.renderSize(), deltaPix);
				_lastPosPix = mouseEvent.outerPoint();
				return true;
			}
			if (mouseEvent.flags() & cv::EVENT_FLAG_RBUTTON) {
				double scalex;
				double scaley;
				scalex = std::pow(2., -deltaPix.x / 100.);
				scaley = std::pow(2., deltaPix.y / 100.);
				_axes.zoom(mouseEvent.renderSize(), _pressPosPix, scalex, scaley);
				_lastPosPix = mouseEvent.outerPoint();
				return true;
			}

		}
		if (mouseEvent.event() == cv::EVENT_RBUTTONDBLCLK) {
			_axes.setXLimAuto();
			_axes.setYLimAuto();
			return true;
		}
		if (mouseEvent.event() == cv::EVENT_MOUSEWHEEL) {
			double scale = std::pow(1.2, - cv::getMouseWheelDelta(mouseEvent.flags()) / 120.);
			_axes.zoom(mouseEvent.renderSize(), mouseEvent.outerPoint(), scale, scale);
			return true;
		}
		return false;
	}
};

CVPLOT_DEFINE_FUN
MouseAdapter::MouseAdapter(Axes &axes)
	:impl(axes){
}


CVPLOT_DEFINE_FUN
MouseAdapter::MouseAdapter(MouseAdapter &&a) 
    :impl(std::move(a.impl)){
}

CVPLOT_DEFINE_FUN
MouseAdapter::~MouseAdapter() {
}

CVPLOT_DEFINE_FUN
Axes &MouseAdapter::getAxes() {
	return impl->_axes;
}

CVPLOT_DEFINE_FUN
bool MouseAdapter::mouseEvent(const MouseEvent &mouseEvent) {
	return impl->mouseEvent(mouseEvent);
}

CVPLOT_DEFINE_FUN
void MouseAdapter::setMouseEventHandler(MouseEventHandler mouseEventHandler){
	impl->_mouseEventHandler = mouseEventHandler;
}


}
