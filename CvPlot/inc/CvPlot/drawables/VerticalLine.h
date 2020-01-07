// CvPlot - https://github.com/Profactor/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <opencv2/core.hpp>
#include <CvPlot/Internal/Pimpl.h>
#include <CvPlot/Internal/no_warning.h>
#include <CvPlot/drawables/LineBase.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE VerticalLine : public LineBaseSub<VerticalLine> {
public:
	VerticalLine(double pos = std::nan(""), const std::string &lineSpec = "-");
	~VerticalLine();
	void render(RenderTarget &renderTarget)override;
	bool getBoundingRect(cv::Rect2d &rect)override;
    VerticalLine& setPos(double pos);
    VerticalLine& setBoundingRectEnabled(bool enabled);
private:
	class Impl;
	CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/VerticalLine.ipp>
#endif