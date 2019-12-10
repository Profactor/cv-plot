// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <CvPlot/core/Axes.h>
#include <vector>

namespace CvPlot {

CVPLOT_LIBRARY_INTERFACE Axes makePlotAxes();
CVPLOT_LIBRARY_INTERFACE Axes makeImageAxes();
CVPLOT_LIBRARY_INTERFACE Axes plot(cv::InputArray data, const std::string &lineSpec = "-");
CVPLOT_LIBRARY_INTERFACE Axes plot(cv::InputArray x, cv::InputArray y, const std::string &lineSpec = "-");
CVPLOT_LIBRARY_INTERFACE Axes plotImage(const cv::Mat &mat);
CVPLOT_LIBRARY_INTERFACE Axes plotImage(const cv::Mat &mat, const cv::Rect2d &position);

}
#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/makeAxes.ipp>
#endif
