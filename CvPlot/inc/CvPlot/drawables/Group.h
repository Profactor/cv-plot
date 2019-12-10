// CvPlot - https://github.com/wpalfi/cv-plot
// Copyright (c) 2019 by PROFACTOR GmbH - https://www.profactor.at/

#pragma once

#include <CvPlot/libdef.h>
#include <CvPlot/core/DrawableDeque.h>
#include <CvPlot/Internal/no_warning.h>

namespace CvPlot {

class CVPLOT_LIBRARY_INTERFACE Group :public DrawableDeque, public Drawable{
public:
	void render(RenderTarget &renderTarget) override;
	bool getBoundingRect(cv::Rect2d &rect) override;
};

}

#ifdef CVPLOT_HEADER_ONLY
#include <CvPlot/imp/Group.ipp>
#endif