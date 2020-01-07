// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/core/makeAxes.h>
#include <CvPlot/drawables/Border.h>
#include <CvPlot/drawables/XAxis.h>
#include <CvPlot/drawables/YAxis.h>
#include <CvPlot/drawables/HorizontalGrid.h>
#include <CvPlot/drawables/VerticalGrid.h>
#include <CvPlot/drawables/Series.h>
#include <CvPlot/drawables/Image.h>
#include <CvPlot/Internal/util.h>

namespace CvPlot {

CVPLOT_DEFINE_FUN
Axes makePlotAxes(){
	Axes axes;
	axes.create<Border>();
	auto& xAxis = axes.create<XAxis>();
	auto& yAxis = axes.create<YAxis>();
	axes.create<VerticalGrid>(&xAxis);
	axes.create<HorizontalGrid>(&yAxis);
	return axes;
}

CVPLOT_DEFINE_FUN
Axes makeImageAxes() {
	Axes axes;
	axes.setFixedAspectRatio();
	axes.setYReverse();
	axes.setXTight();
	axes.setYTight();
	axes.setTightBox();
	axes.create<Border>();
	axes.create<XAxis>();
	axes.create<YAxis>();
	return axes;
}

CVPLOT_DEFINE_FUN
Axes plot(cv::InputArray data, const std::string &lineSpec) {
	Axes axes = makePlotAxes();
	axes.create<Series>(data, lineSpec);
	return axes;
}

CVPLOT_DEFINE_FUN
Axes plot(cv::InputArray x, cv::InputArray y, const std::string &lineSpec){
	Axes axes = makePlotAxes();
	axes.create<Series>(x, y, lineSpec);
	return axes;
}

CVPLOT_DEFINE_FUN
Axes plotImage(const cv::Mat &mat) {
	Axes axes = makeImageAxes();
	axes.create<Image>(mat);
	return axes;
}

CVPLOT_DEFINE_FUN
Axes plotImage(const cv::Mat &mat, const cv::Rect2d &position) {
	Axes axes = makeImageAxes();
	axes.create<Image>(mat, position);
	return axes;
}

}