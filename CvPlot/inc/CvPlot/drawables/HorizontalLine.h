// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/Internal/Pimpl.h>
#include <CvPlot/Internal/no_warning.h>
#include <CvPlot/drawables/LineBase.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE HorizontalLine : public LineBaseSub<HorizontalLine> {
public:
	HorizontalLine(double pos = std::nan(""), const std::string &lineSpec = "-");
	~HorizontalLine();
	void render(RenderTarget &renderTarget)override;
	bool getBoundingRect(cv::Rect2d &rect)override;
	HorizontalLine& setPos(double pos);
    HorizontalLine& setBoundingRectEnabled(bool enabled);
private:
	class Impl;
	CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/HorizontalLine.ipp>
#endif