// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <CvPlot/internal/Pimpl.h>
#include <CvPlot/internal/no_warning.h>
#include <CvPlot/core/Drawable.h>
#include <CvPlot/drawables/XAxis.h>
#include <CvPlot/drawables/LineBase.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE VerticalGrid : public LineBaseSub<VerticalGrid> {
public:
	VerticalGrid(XAxis* xAxis=nullptr);
	~VerticalGrid();
	VerticalGrid& setXAxis(XAxis* xAxis);
	void render(RenderTarget &renderTarget)override;
	bool getBoundingRect(cv::Rect2d &rect)override;
	VerticalGrid& setEnabled(bool enabled);
	bool getEnabled();
private:
	class Impl;
	CVPLOT_NO_WARNING_DLL_INTERFACE(Internal::Pimpl<Impl>, impl);
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/VerticalGrid.ipp>
#endif