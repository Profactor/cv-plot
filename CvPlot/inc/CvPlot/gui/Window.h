// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <CvPlot/core/Axes.h>
#include <CvPlot/gui/MouseAdapter.h>
#include <string>
#include <opencv2/highgui.hpp>
#include <algorithm>

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
    bool valid() const;
    int waitKey(int delay = 0);
    static int waitKey(const std::vector<Window*> &windows, int delay = 0);
private:
    MouseAdapter _mouseAdapter;
    std::string _windowName;
    cv::Mat3b _mat;
    void updateSize();
	void setMouseCallback();
};

inline
Window::Window(std::string windowName, Axes &axes, int rows, int cols)
    :_mouseAdapter(axes)
    , _windowName(windowName) {

    if(valid()){
        cv::destroyWindow(windowName);
    }
    cv::namedWindow(windowName, cv::WINDOW_NORMAL | cv::WINDOW_FREERATIO);
    cv::resizeWindow(windowName, { cols,rows });
    axes.render(_mat, cv::Size(cols, rows));
    cv::imshow(windowName, _mat);
	setMouseCallback();
}

inline
Window::Window(Window && a)
    :_mouseAdapter(std::move(a._mouseAdapter))
    , _windowName(std::move(a._windowName))
    , _mat(std::move(a._mat) ){
	setMouseCallback();
}

inline
Window::~Window() {
    if(valid()){
        cv::destroyWindow(_windowName);
    }
}

inline
Axes& Window::getAxes() {
    return _mouseAdapter.getAxes();
}

inline
void Window::update() {
    if(!valid()){
        return;
    }
    auto rect = cv::getWindowImageRect(_windowName);
    _mouseAdapter.getAxes().render(_mat, rect.size());
    if (!_mat.empty()) {
        cv::imshow(_windowName, _mat);
    }
}

inline
void Window::setMouseEventHandler(MouseEventHandler mouseEventHandler) {
    _mouseAdapter.setMouseEventHandler(mouseEventHandler);
}

inline
bool Window::valid() const {
    try {
        return getWindowProperty(_windowName, cv::WND_PROP_AUTOSIZE) >= 0;
    } catch (...) {
        return false;
    }
}

inline
int Window::waitKey(int delay){
    return waitKey({ this }, delay);
}

inline 
int Window::waitKey(const std::vector<Window*> &windows, int delay) {
    const auto endTickCount = cv::getTickCount() + (int64)(delay * 0.001f * cv::getTickFrequency());

    while (true) {
        //wait for short period
        int key = cv::waitKey(1);
        if (key != -1) {
            return key;
        }

        //timed out?
        if (delay > 0 && cv::getTickCount() - endTickCount >= 0) {
            return -1;
        }

        //all windows closed?
        if (std::all_of(windows.begin(), windows.end(), [](Window *window) {
            return !window->valid();
        })) {
            return -1;
        }

        //update window sizes
        for (auto window : windows) {
            window->updateSize();
        }
    }
}

inline
void Window::updateSize() {
    if (!valid()) {
        return;
    }
    auto rect = cv::getWindowImageRect(_windowName);
    if(rect.size() != _mat.size()) {
        update();
    }
}

inline
void Window::setMouseCallback() {
	cv::setMouseCallback(_windowName, [](int event, int x, int y, int flags, void* userdata) {
		Window& window = *static_cast<Window*>(userdata);
		window.updateSize();
		MouseEvent mouseEvent(window._mouseAdapter.getAxes(), window._mat.size(), event, x, y, flags);
		if (window._mouseAdapter.mouseEvent(mouseEvent)) {
			window.update();
		}
		}, this);
}

}

