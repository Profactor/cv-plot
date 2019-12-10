// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/Internal/no_warning.h>
#include <CvPlot/core/Drawable.h>
#include <CvPlot/core/DrawableDeque.h>
#include <CvPlot/core/Projection.h>
#include <CvPlot/Internal/Pimpl.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE Axes :public DrawableDeque {
public:
	Axes();
	~Axes();
	Axes(Axes &&);
	Axes& operator=(Axes&&);
	
	Axes& setMargins(int left, int right, int top, int bottom);
	
	cv::Mat3b render(int rows = 480, int cols = 640)const;
	void render(cv::Mat &mat)const;
	void render(cv::Mat &mat, cv::Size size)const;
	
	Axes& xLabel(const std::string &label);
	Axes& yLabel(const std::string &label);
	Axes& title(const std::string &title);
	Axes& enableHorizontalGrid(bool enable = true);
	Axes& enableVerticalGrid(bool enable = true);

	Axes& setXLim(std::pair<double, double> xlim);
	std::pair<double, double> getXLim()const;
	Axes& setYLim(std::pair<double, double> xlim);
	std::pair<double, double> getYLim()const;
	Axes& setXLimAuto(bool xLimAuto = true);
	bool getXLimAuto()const;
	Axes& setYLimAuto(bool yLimAuto = true);
	bool getYLimAuto()const;
	Axes& setYReverse(bool reverse = true);
	bool getYReverse(void)const;
    Axes& setFixedAspectRatio(bool fixed = true);
    bool getFixedAspectRatio()const;
    Axes& setAspectRatio(double aspectRatio);
    double getAspectRatio()const;
	Axes& setXTight(bool tight = true);
	bool getXTight()const;
	Axes& setYTight(bool tight = true);
	bool getYTight()const;
	Axes& setTightBox(bool tight = true);
	bool getTightBox()const;
	Axes& setTransformation(std::unique_ptr<Transformation> transformation);
	Axes& setXLog(bool log = true);
	bool getXLog()const;
	Axes& setYLog(bool log = true);
	bool getYLog()const;

	void zoom(cv::Size size, cv::Point outerPos, double scalex, double scaley);
	void pan(cv::Size size, cv::Point delta);
	Projection getProjection(cv::Size size)const;

private:
	class Impl;
	CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/Axes.ipp>
#endif
