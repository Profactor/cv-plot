// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <CvPlot/core/makeAxes.h>
#include <CvPlot/gui/Window.h>
#include <opencv2/highgui.hpp>

namespace CvPlot {
	
//some convenience functions for lazy developers

inline void show(const std::string &windowName, Axes &axes, int rows = 480, int cols = 640) {
	Window window(windowName, axes, rows, cols);
	cv::waitKey();
}

//arguments: see Series constructor
inline void showPlot(cv::InputArray x, cv::InputArray y, const std::string &lineSpec = "-") {
    auto axes = plot(x, y, lineSpec);
	show("CvPlot", axes);
}

//arguments: see Series constructor
inline void showPlot(cv::InputArray data, const std::string &lineSpec = "-") {
    auto axes = plot(data, lineSpec);
	show("CvPlot", axes);
}

inline void showImage(const cv::Mat &mat) {
    auto axes = plotImage(mat);
	show("CvPlot", axes);
}

}
