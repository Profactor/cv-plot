// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <CvPlot/core/Axes.h>
#include <CvPlot/gui/MouseAdapter.h>
#include <string>
#include <opencv2/highgui.hpp>

namespace CvPlot {

//Window is always header-only to avoid problems with static opencv
class Window {
public:
	Window(std::string windowName, Axes &axes, int rows = 480, int cols = 640);
	Window(Window&& a);
	~Window();
	Axes& getAxes();
	void update();
	void setMouseEventHandler(MouseEventHandler mouseEventHandler);
private:
    MouseAdapter _mouseAdapter;
    std::string _windowName;
    cv::Mat3b _mat;
};

inline
Window::Window(std::string windowName, Axes &axes, int rows, int cols)
    :_mouseAdapter(axes)
    , _windowName(windowName) {

    cv::destroyWindow(windowName); //destroy opencv window if existing
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE); //resizing not supported (no way to detect window size changes)
    axes.render(_mat, cv::Size(cols, rows));
    cv::imshow(windowName, _mat);
    cv::setMouseCallback(windowName, [](int event, int x, int y, int flags, void* userdata) {
        Window& window = *static_cast<Window*>(userdata);
        MouseEvent mouseEvent(window._mouseAdapter.getAxes(), window._mat.size(), event, x, y, flags);
        if (window._mouseAdapter.mouseEvent(mouseEvent)) {
            window.update();
        }
        }, this);
}

inline
Window::Window(Window && a)
    :_mouseAdapter(std::move(a._mouseAdapter))
    , _windowName(std::move(a._windowName))
    , _mat(std::move(a._mat) ){
}

inline
Window::~Window() {
    cv::destroyWindow(_windowName);
}

inline
Axes& Window::getAxes() {
    return _mouseAdapter.getAxes();
}

inline
void Window::update() {
    _mouseAdapter.getAxes().render(_mat);
    cv::imshow(_windowName, _mat);
}

inline
void Window::setMouseEventHandler(MouseEventHandler mouseEventHandler) {
    _mouseAdapter.setMouseEventHandler(mouseEventHandler);
}

}

