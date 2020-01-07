// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/Internal/no_warning.h>
#include <CvPlot/Internal/Pimpl.h>
#include <CvPlot/core/Drawable.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE YAxis : public Drawable {
public:
	YAxis();
	~YAxis();
	void render(RenderTarget &renderTarget)override;
	bool getBoundingRect(cv::Rect2d &rect)override;
	const std::vector<double> &getTicks()const;
	int getWidth()const;
	YAxis& setLocateRight(bool locateRight = true);
private:
	class Impl;
	CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/YAxis.ipp>
#endif
